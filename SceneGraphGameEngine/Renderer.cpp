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
	/*mAmbientShader = new ShaderProgram("Resources//Shaders//forward_ambient");
	mDirectionalShader = new ShaderProgram("Resources//Shaders//forward_directional");
	mPointShader = new ShaderProgram("Resources//Shaders//forward_point");
	mAmbientShaderTerrain = new ShaderProgram("Resources//Shaders//forward_ambient", "#define MULTI_TEXTURE\n");
	mDirectionalShaderTerrain = new ShaderProgram("Resources//Shaders//forward_directional", "#define MULTI_TEXTURE\n");
	mPointShaderTerrain = new ShaderProgram("Resources//Shaders//forward_point", "#define MULTI_TEXTURE\n");*/
	mShaders[SKYBOX] = new ShaderProgram("Resources//Shaders//skybox");
	
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

	if (!(shader&SKYBOX) && !(shader&WATER))
	{
		//make sure the shaders are loaded
		if (mShaders.count(AMBIENT | shader) == 0)
		{
			std::string defs = getDefsFromShaderType(shader);
			mShaders[AMBIENT | shader] = new ShaderProgram("Resources//Shaders//forward_ambient", defs);
			mShaders[DIRECTIONAL | shader] = new ShaderProgram("Resources//Shaders//forward_directional_tangent", defs);
			mShaders[POINT | shader] = new ShaderProgram("Resources//Shaders//forward_point", defs);

			if (shader&NORMAL_MAP)
			{
				mShaders[DIRECTIONAL | shader]->bind();
				mShaders[DIRECTIONAL | shader]->setTextureLocation("gTexture", 0);
				mShaders[DIRECTIONAL | shader]->setTextureLocation("gNormalMap", 1);
				mShaders[POINT | shader]->bind();
				mShaders[POINT | shader]->setTextureLocation("gTexture", 0);
				mShaders[POINT | shader]->setTextureLocation("gNormalMap", 1);
			}
		}

		//add to renderables
		mRenderables[shader | AMBIENT].push_back(ro);
		mRenderables[shader | DIRECTIONAL].push_back(ro);
		mRenderables[shader | POINT].push_back(ro);
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

void Renderer::renderShader(int shader, const glm::mat4& view, const glm::mat4& projection)
{
	if (mShaders.count(shader) == 0 || mRenderables[shader].size() == 0)
	{
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
					mRenderables[shader][i].mMaterial->mNormalMap->bind(GL_TEXTURE1);
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

void Renderer::renderScene(const glm::mat4& view, const glm::mat4& projection)
{
	glDepthMask(true);
	glDisable(GL_BLEND);
	glActiveTexture(GL_TEXTURE0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	
	renderSkybox(view, projection);

	glDepthFunc(GL_LESS);

	/*mAmbientShader->bind();
	mAmbientShader->setUniformMat4f("gViewMat", view);
	mAmbientShader->setUniformMat4f("gProjectionMat", projection);
	for (size_t i = 0; i < mRenderObjects.size(); i++)
	{
		mRenderObjects[i].mMaterial->mTexture->bind();
		mAmbientShader->setUniformMat4f("gModelMat", *mRenderObjects[i].mModelMat);
		mRenderObjects[i].mMesh->render();
	}*/

	/*mAmbientShaderTerrain->bind();
	mAmbientShader->setUniformMat4f("gViewMat", view);
	mAmbientShader->setUniformMat4f("gProjectionMat", projection);
	for (size_t i = 0; i < mRenderObjects.size(); i++)
	{
		mRenderObjects[i].mMaterial->mTexture->bind();
		mAmbientShader->setUniformMat4f("gModelMat", *mRenderObjects[i].mModelMat);
		mRenderObjects[i].mMesh->render();
	}*/

	renderShader(AMBIENT, view, projection);
	renderShader(AMBIENT | NORMAL_MAP, view, projection);
	renderShader(AMBIENT | MULTI_TEXTURE, view, projection);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	glDepthMask(false);
	glDepthFunc(GL_EQUAL);
	renderShader(DIRECTIONAL, view, projection);
	renderShader(DIRECTIONAL | NORMAL_MAP, view, projection);
	renderShader(DIRECTIONAL | MULTI_TEXTURE, view, projection);

	renderShader(POINT, view, projection);
	renderShader(POINT | NORMAL_MAP, view, projection);
	renderShader(POINT | MULTI_TEXTURE, view, projection);

	/*mDirectionalShader->bind();
	mDirectionalShader->setUniformMat4f("gViewMat", view);
	mDirectionalShader->setUniformMat4f("gProjectionMat", projection);
	for (size_t i = 0; i < mDirectionalLights.size(); i++)
	{
		mDirectionalShader->setUniformVec3f("gLight.dir", mDirectionalLights[i].dir);
		mDirectionalShader->setUniformVec3f("gLight.color", mDirectionalLights[i].color);
		mDirectionalShader->setUniformFloat("gLight.intensity", mDirectionalLights[i].intensity);

		for (size_t i = 0; i < mRenderObjects.size(); i++)
		{
			mRenderObjects[i].mMaterial->mTexture->bind();
			mDirectionalShader->setUniformMat4f("gModelMat", *mRenderObjects[i].mModelMat);
			mDirectionalShader->setUniformFloat("gSpecularIntensity", mRenderObjects[i].mMaterial->mSpecularIntensity);
			mDirectionalShader->setUniformFloat("gShineDamper", mRenderObjects[i].mMaterial->mShineDamper);
			mRenderObjects[i].mMesh->render();
		}
	}*/

	/*mDirectionalShaderTerrain->bind();
	mDirectionalShaderTerrain->setUniformMat4f("gViewMat", view);
	mDirectionalShaderTerrain->setUniformMat4f("gProjectionMat", projection);
	for (size_t i = 0; i < mDirectionalLights.size(); i++)
	{
		mDirectionalShaderTerrain->setUniformVec3f("gLight.dir", mDirectionalLights[i].dir);
		mDirectionalShaderTerrain->setUniformVec3f("gLight.color", mDirectionalLights[i].color);
		mDirectionalShaderTerrain->setUniformFloat("gLight.intensity", mDirectionalLights[i].intensity);

		for (size_t i = 0; i < mRenderObjects.size(); i++)
		{
			mRenderObjects[i].mMaterial->mTexture->bind();
			mDirectionalShaderTerrain->setUniformMat4f("gModelMat", *mRenderObjects[i].mModelMat);
			mDirectionalShaderTerrain->setUniformFloat("gSpecularIntensity", mRenderObjects[i].mMaterial->mSpecularIntensity);
			mDirectionalShaderTerrain->setUniformFloat("gShineDamper", mRenderObjects[i].mMaterial->mShineDamper);
			mRenderObjects[i].mMesh->render();
		}
	}*/

	/*mPointShader->bind();
	mPointShader->setUniformMat4f("gViewMat", view);
	mPointShader->setUniformMat4f("gProjectionMat", projection);
	for (size_t i = 0; i < mPointLights.size(); i++)
	{
		mPointShader->setUniformVec3f("gLight.pos", mPointLights[i].pos);
		mPointShader->setUniformVec3f("gLight.color", mPointLights[i].color);
		mPointShader->setUniformVec3f("gLight.attenuation", mPointLights[i].attenuation);

		for (size_t i = 0; i < mRenderObjects.size(); i++)
		{
			mRenderObjects[i].mMaterial->mTexture->bind();
			mPointShader->setUniformMat4f("gModelMat", *mRenderObjects[i].mModelMat);
			mPointShader->setUniformFloat("gSpecularIntensity", mRenderObjects[i].mMaterial->mSpecularIntensity);
			mPointShader->setUniformFloat("gShineDamper", mRenderObjects[i].mMaterial->mShineDamper);
			mRenderObjects[i].mMesh->render();
		}
	}*/

	/*mPointShaderTerrain->bind();
	mPointShaderTerrain->setUniformMat4f("gViewMat", view);
	mPointShaderTerrain->setUniformMat4f("gProjectionMat", projection);
	for (size_t i = 0; i < mPointLights.size(); i++)
	{
		mPointShaderTerrain->setUniformVec3f("gLight.pos", mPointLights[i].pos);
		mPointShaderTerrain->setUniformVec3f("gLight.color", mPointLights[i].color);
		mPointShaderTerrain->setUniformVec3f("gLight.attenuation", mPointLights[i].attenuation);

		for (size_t i = 0; i < mRenderObjects.size(); i++)
		{
			mRenderObjects[i].mMaterial->mTexture->bind();
			mPointShaderTerrain->setUniformMat4f("gModelMat", *mRenderObjects[i].mModelMat);
			mPointShaderTerrain->setUniformFloat("gSpecularIntensity", mRenderObjects[i].mMaterial->mSpecularIntensity);
			mPointShaderTerrain->setUniformFloat("gShineDamper", mRenderObjects[i].mMaterial->mShineDamper);
			mRenderObjects[i].mMesh->render();
		}
	}*/
}

void Renderer::render()
{
	//mShadowFBO.unbind();
	assert(mCamera != NULL);
	glm::mat4 model, view, projection;
	mCamera->render(view, projection);
	renderScene(view, projection);

	mCamera->mPosition = glm::vec3(mCamera->mPosition.x, -mCamera->mPosition.y, mCamera->mPosition.z);
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
	mWater->render();
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
	return defs;
}
