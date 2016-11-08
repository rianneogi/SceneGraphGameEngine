#include "Renderer.h"

Renderer::Renderer() : mCamera(NULL)
{
}

Renderer::Renderer(Camera * cam) : mCamera(cam)
{
}

Renderer::~Renderer()
{
	for (auto i = mShaders.begin(); i != mShaders.end(); i++)
	{
		if (i->second != NULL)
		{
			delete i->second;
		}
	}
}

void Renderer::loadShaders()
{
	mShaders[SKYBOX] = new ShaderProgram("Resources//Shaders//skybox");
	mShaders[SHADOW_MAP] = new ShaderProgram("Resources//Shaders//shadow_pass");
	
	assert(mWater != NULL && mCamera != NULL);
	mShaders[WATER] = new ShaderProgram("Resources//Shaders//water");
	mShaders[WATER]->bind();
	mShaders[WATER]->setUniformFloat("gWaveStrength", mWater->mWaveStrength);
	mShaders[WATER]->setUniformFloat("gWaveHeight", mWater->mWaveHeight);
	mShaders[WATER]->setUniformFloat("gSpecularFactor", mWater->mSpecularFactor);
	mShaders[WATER]->setUniformFloat("gShineDamper", mWater->mShineDamper);
	mShaders[WATER]->setUniformFloat("gScreenNear", mCamera->mScreenNear);
	mShaders[WATER]->setUniformFloat("gScreenFar", mCamera->mScreenFar);
	mShaders[WATER]->setTextureLocation("gReflectionTexture", 0);
	mShaders[WATER]->setTextureLocation("gRefractionTexture", 1);
	mShaders[WATER]->setTextureLocation("gDuDv", 2);
	mShaders[WATER]->setTextureLocation("gDepthTexture", 3);

	mShadowFBO.initFBO(1024, 1024, GL_NONE);
	mShadowFBO.createDepthTextureAttachment();
	mShadowFBO.checkStatus();
	mShadowFBO.unbind();
}

void Renderer::addMesh(MeshDataTex * mesh)
{
	mMeshes.push_back(mesh);
}

void Renderer::addTexture(Texture2D * tex)
{
	mTextures.push_back(tex);
}

void Renderer::addRenderObject(MeshData * mesh, Material * tex, int shader, glm::mat4* model)
{
	RenderObject ro = RenderObject(mesh, tex, shader, model);
	mRenderObjects.push_back(ro);

	printf("adding to renderer: %d", shader);

	if (!(shader&SKYBOX) && !(shader&WATER))
	{
		//make sure the shaders are loaded
		if (mShaders.count(AMBIENT | shader) == 0)
		{
			std::string defs = getDefsFromShaderType(shader);
			mShaders[AMBIENT | shader] = new ShaderProgram("Resources//Shaders//forward_ambient", defs);
			mShaders[DIRECTIONAL | SHADOW_MAP | shader] = new ShaderProgram("Resources//Shaders//forward_directional_tangent", defs+"#define SHADOW_MAP\n");
			mShaders[POINT | shader] = new ShaderProgram("Resources//Shaders//forward_point", defs);
			printf("creating shader %d\n", DIRECTIONAL | SHADOW_MAP | shader);
			mShaders[DIRECTIONAL | SHADOW_MAP | shader]->bind();
			mShaders[DIRECTIONAL | SHADOW_MAP | shader]->setTextureLocation("gTexture", 0);
			//if (shader&SHADOW_MAP)
			{
				mShaders[DIRECTIONAL | SHADOW_MAP | shader]->setTextureLocation("gShadowMap", 1);
			}
			if (shader&NORMAL_MAP)
			{
				mShaders[DIRECTIONAL | SHADOW_MAP | shader]->setTextureLocation("gNormalMap", 2);
			}
			if (shader&PARALLAX_MAP)
			{
				mShaders[DIRECTIONAL | SHADOW_MAP | shader]->setTextureLocation("gDisplacementMap", 3);
			}
			
			mShaders[POINT | shader]->bind();
			mShaders[POINT | shader]->setTextureLocation("gTexture", 0);
			if (shader&SHADOW_MAP)
			{
				mShaders[POINT | shader]->setTextureLocation("gShadowMap", 1);
			}
			if (shader&NORMAL_MAP)
			{
				mShaders[POINT | shader]->setTextureLocation("gNormalMap", 2);
			}
			if (shader&PARALLAX_MAP)
			{
				mShaders[POINT | shader]->setTextureLocation("gDisplacementMap", 3);
			}
		}

		//add to renderables
		mRenderables[shader | AMBIENT].push_back(ro);
		mRenderables[shader | DIRECTIONAL | SHADOW_MAP].push_back(ro);
		mRenderables[shader | POINT].push_back(ro);
		//if (!(shader&MULTI_TEXTURE))
			mRenderables[SHADOW_MAP].push_back(ro);
	}
}

void Renderer::addPointLight(const PointLight & l)
{
	mPointLights.push_back(l);
}

void Renderer::addDirectionalLight(const DirectionalLight & l)
{
	mDirectionalLights.push_back(l);
}

void Renderer::renderSkybox(const glm::mat4& view, const glm::mat4& projection)
{
	glCullFace(GL_FRONT);
	glDepthFunc(GL_LEQUAL);
	
	assert(mSkyBox != NULL);
	assert(mSkyBox->mMesh != NULL && mSkyBox->mTexture != NULL);

	mShaders[SKYBOX]->bind();
	mShaders[SKYBOX]->setUniformMat4f("gModelMat", glm::translate(glm::mat4(1.0), mCamera->mPosition));
	mShaders[SKYBOX]->setUniformMat4f("gViewMat", view);
	mShaders[SKYBOX]->setUniformMat4f("gProjectionMat", projection);
	mSkyBox->mTexture->bind();
	mSkyBox->mMesh->render();

	glCullFace(GL_BACK);
}

void Renderer::renderShader(int shader, const glm::mat4& view, const glm::mat4& projection, const glm::mat4& lightView, const glm::mat4& lightProj)
{
	if (mShaders.count(shader) == 0 || mRenderables[shader].size() == 0)
	{
		//printf("bad shader: %d\n", shader);
		return;
	}

	mShaders[shader]->bind();
	mShaders[shader]->setUniformMat4f("gViewMat", view);
	mShaders[shader]->setUniformMat4f("gProjectionMat", projection);

	if (shader&AMBIENT)
	{
		for (size_t i = 0; i < mRenderables[shader].size(); i++)
		{
			mRenderables[shader][i].mMaterial->mTexture->bind();
			mShaders[shader]->setUniformMat4f("gModelMat", *mRenderables[shader][i].mModelMat);
			mRenderables[shader][i].mMesh->render();
		}
	}
	else if (shader&DIRECTIONAL)
	{
		for (size_t i = 0; i < mDirectionalLights.size(); i++)
		{
			mShaders[shader]->setUniformVec3f("gLight.dir", mDirectionalLights[i].dir);
			mShaders[shader]->setUniformVec3f("gLight.color", mDirectionalLights[i].color);
			mShaders[shader]->setUniformFloat("gLight.intensity", mDirectionalLights[i].intensity);

			for (size_t i = 0; i < mRenderables[shader].size(); i++)
			{
				mRenderables[shader][i].mMaterial->mTexture->bind();
				mShaders[shader]->setUniformMat4f("gModelMat", *mRenderables[shader][i].mModelMat);
				mShaders[shader]->setUniformFloat("gSpecularIntensity", mRenderables[shader][i].mMaterial->mSpecularIntensity);
				mShaders[shader]->setUniformFloat("gShineDamper", mRenderables[shader][i].mMaterial->mShineDamper);
				if (shader&NORMAL_MAP)
				{
					mRenderables[shader][i].mMaterial->mNormalMap->bind(GL_TEXTURE2);
				}
				if (shader&SHADOW_MAP)
				{
					//mShadowFBO.bindDepthTexture(GL_TEXTURE0);
					glActiveTexture(GL_TEXTURE1);
					GLint drawFboId = 0;
					glGetIntegerv(GL_TEXTURE_BINDING_2D, &drawFboId);
					printf("Rendering with: %d %d\n", drawFboId, mShadowFBO.mDepthTexture);
					glm::mat4 biasMatrix(
						0.5, 0.0, 0.0, 0.0,
						0.0, 0.5, 0.0, 0.0,
						0.0, 0.0, 0.5, 0.0,
						0.5, 0.5, 0.5, 1.0
					);
					glm::mat4 depthBiasMVP = biasMatrix*lightProj*lightView;
					mShaders[shader]->setUniformMat4f("gDepthBiasMVP", depthBiasMVP);
				}
				
				mRenderables[shader][i].mMesh->render();
			}
		}
	}
	else if (shader&POINT)
	{
		for (size_t i = 0; i < mPointLights.size(); i++)
		{
			mShaders[shader]->setUniformVec3f("gLight.pos", mPointLights[i].pos);
			mShaders[shader]->setUniformVec3f("gLight.color", mPointLights[i].color);
			mShaders[shader]->setUniformVec3f("gLight.attenuation", mPointLights[i].attenuation);

			for (size_t i = 0; i < mRenderables[shader].size(); i++)
			{
				mRenderables[shader][i].mMaterial->mTexture->bind();
				mShaders[shader]->setUniformMat4f("gModelMat", *mRenderables[shader][i].mModelMat);
				mShaders[shader]->setUniformFloat("gSpecularIntensity", mRenderables[shader][i].mMaterial->mSpecularIntensity);
				mShaders[shader]->setUniformFloat("gShineDamper", mRenderables[shader][i].mMaterial->mShineDamper);
				if (shader&NORMAL_MAP)
				{
					mRenderables[shader][i].mMaterial->mNormalMap->bind(GL_TEXTURE1);
				}
				mRenderables[shader][i].mMesh->render();
			}
		}
	}
}

void Renderer::doShadowPass(glm::mat4& lightView, glm::mat4& lightProj)
{
	glDepthMask(true);
	glDepthFunc(GL_LESS);

	float gSunDistance = 100.f;

	mShaders[SHADOW_MAP]->bind();
	mShadowFBO.bindForWriting();
	GLint drawFboId = 0, readFboId = 0;
	glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &drawFboId);
	glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &readFboId);
	printf("Rendering with: %d %d %d\n", drawFboId, readFboId, mShadowFBO);
	
	lightView = glm::lookAt((-mDirectionalLights[0].dir)*gSunDistance, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	lightProj = glm::ortho<float>(-2 * gSunDistance, 2 * gSunDistance, -2 * gSunDistance, 2 * gSunDistance, -2 * gSunDistance, 4 * gSunDistance);
	mShaders[SHADOW_MAP]->setUniformMat4f("gViewMat", lightView);
	mShaders[SHADOW_MAP]->setUniformMat4f("gProjectionMat", lightProj);
	for (size_t i = 0; i < mRenderables[SHADOW_MAP].size(); i++)
	{
		mRenderables[SHADOW_MAP][i].mMaterial->mTexture->bind();
		mShaders[SHADOW_MAP]->setUniformMat4f("gModelMat", *mRenderables[SHADOW_MAP][i].mModelMat);
		mRenderables[SHADOW_MAP][i].mMesh->render();
	}

	mShadowFBO.unbind();
}

void Renderer::renderScene(const glm::mat4& view, const glm::mat4& projection)
{
	glDepthMask(true);
	glDisable(GL_BLEND);
	glActiveTexture(GL_TEXTURE0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glm::mat4 lightView;
	glm::mat4 lightProj;
	doShadowPass(lightView, lightProj);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	
	renderSkybox(view, projection);

	glDepthFunc(GL_LESS);

	renderShader(AMBIENT, view, projection, lightView, lightProj);
	renderShader(AMBIENT | NORMAL_MAP, view, projection, lightView, lightProj);
	renderShader(AMBIENT | MULTI_TEXTURE, view, projection, lightView, lightProj);

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	glDepthMask(false);
	glDepthFunc(GL_EQUAL);

	mShadowFBO.bindDepthTexture(GL_TEXTURE1);
	renderShader(DIRECTIONAL, view, projection, lightView, lightProj);
	renderShader(DIRECTIONAL | NORMAL_MAP | SHADOW_MAP, view, projection, lightView, lightProj);
	renderShader(DIRECTIONAL | MULTI_TEXTURE | SHADOW_MAP, view, projection, lightView, lightProj);

	renderShader(POINT, view, projection, lightView, lightProj);
	renderShader(POINT | NORMAL_MAP, view, projection, lightView, lightProj);
	renderShader(POINT | MULTI_TEXTURE, view, projection, lightView, lightProj);
}

void Renderer::render()
{
	assert(mCamera != NULL);
	glm::mat4 model, view, projection;
	mCamera->render(view, projection);
	renderScene(view, projection);

	/*mCamera->mPosition = glm::vec3(mCamera->mPosition.x, -mCamera->mPosition.y, mCamera->mPosition.z);
	mCamera->mVerticalAngle = -mCamera->mVerticalAngle;
	mCamera->render(view, projection);
	mWater->bindReflection();
	renderScene(view, projection);

	mCamera->mPosition = glm::vec3(mCamera->mPosition.x, -mCamera->mPosition.y, mCamera->mPosition.z);
	mCamera->mVerticalAngle = -mCamera->mVerticalAngle;
	mCamera->render(view, projection);
	mWater->bindRefraction();
	renderScene(view, projection);
	mWater->mRefractionFBO.unbind();

	renderScene(view, projection);

	mWater->mReflectionFBO.bindColorTexture(GL_TEXTURE0);
	mWater->mRefractionFBO.bindColorTexture(GL_TEXTURE1);
	mWater->mDuDvTexture->bind(GL_TEXTURE2);
	mWater->mRefractionFBO.bindDepthTexture(GL_TEXTURE3);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(true);
	glDepthFunc(GL_LESS);
	
	assert(mShaders[WATER] != NULL);
	mShaders[WATER]->bind();
	mShaders[WATER]->setUniformMat4f("gModelMat", glm::scale(glm::vec3(160, 160, 160)));
	mShaders[WATER]->setUniformMat4f("gViewMat", view);
	mShaders[WATER]->setUniformMat4f("gProjectionMat", projection);
	mShaders[WATER]->setUniformFloat("gDuDvOffset", mWater->mDuDvOffset);
	mShaders[WATER]->setUniformVec3f("gEyePos", mCamera->mPosition);
	mShaders[WATER]->setUniformVec3f("gLightDir", mDirectionalLights[0].dir);
	mWater->render();*/
}

std::string getDefsFromShaderType(int shader)
{
	std::string defs = "";
	if(shader&MULTI_TEXTURE)
	{
		defs += "#define MULTI_TEXTURE\n";
	}
	if (shader&NORMAL_MAP)
	{
		defs += "#define NORMAL_MAP\n";
	}
	if (shader&PARALLAX_MAP)
	{
		defs += "#define PARALLAX_MAP\n";
	}
	if (shader&SPECULAR_MAP)
	{
		defs += "#define SPECULAR_MAP\n";
	}
	if (shader&SOLID_COLOR)
	{
		defs += "#define SOLID_COLOR\n";
	}
	if (shader&SHADOW_MAP)
	{
		defs += "#define SHADOW_MAP\n";
	}
	return defs;
}
