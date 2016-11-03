#include "Renderer.h"

Renderer::Renderer() : mCamera(NULL)
{
}

Renderer::Renderer(Camera * cam) : mCamera(cam)
{
}

Renderer::~Renderer()
{
	for (int i = 0; i < mShaders.size(); i++)
	{
		if (mShaders[i] != NULL)
		{
			delete mShaders[i];
		}
	}
}

void Renderer::loadShaders()
{
	mAmbientShader = new ShaderProgram("Resources//Shaders//forward_ambient");
	mDirectionalShader = new ShaderProgram("Resources//Shaders//forward_directional");
	mPointShader = new ShaderProgram("Resources//Shaders//forward_point");
	mAmbientShaderTerrain = new ShaderProgram("Resources//Shaders//forward_ambient", "#define MULTI_TEXTURE\n");
	mDirectionalShaderTerrain = new ShaderProgram("Resources//Shaders//forward_directional", "#define MULTI_TEXTURE\n");
	mPointShaderTerrain = new ShaderProgram("Resources//Shaders//forward_point", "#define MULTI_TEXTURE\n");
	mSkyboxShader = new ShaderProgram("Resources//Shaders//skybox");
	
	assert(mWater != NULL && mCamera != NULL);
	mWaterShader = new ShaderProgram("Resources//Shaders//water");
	mWaterShader->bind();
	mWaterShader->setUniformFloat("gWaveStrength", mWater->mWaveStrength);
	mWaterShader->setUniformFloat("gWaveHeight", mWater->mWaveHeight);
	mWaterShader->setUniformFloat("gSpecularFactor", mWater->mSpecularFactor);
	mWaterShader->setUniformFloat("gShineDamper", mWater->mShineDamper);
	mWaterShader->setUniformFloat("gScreenNear", mCamera->mScreenNear);
	mWaterShader->setUniformFloat("gScreenFar", mCamera->mScreenFar);
	mWaterShader->setTextureLocation("gReflectionTexture", 0);
	mWaterShader->setTextureLocation("gRefractionTexture", 1);
	mWaterShader->setTextureLocation("gDuDv", 2);
	mWaterShader->setTextureLocation("gDepthTexture", 3);
}

void Renderer::addMesh(MeshDataTex * mesh)
{
	mMeshes.push_back(mesh);
}

void Renderer::addTexture(Texture * tex)
{
	mTextures.push_back(tex);
}

void Renderer::addShader(ShaderProgram * shader)
{
	mShaders.push_back(shader);
}

void Renderer::addRenderObject(Mesh * mesh, Material * tex, SHADER_PIPELINE shader, glm::mat4* model)
{
	mRenderObjects.push_back(RenderObject(mesh, tex, shader, model));
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

	mSkyboxShader->bind();
	mSkyboxShader->setUniformMat4f("gModelMat", glm::translate(glm::mat4(1.0), mCamera->mPosition));
	mSkyboxShader->setUniformMat4f("gViewMat", view);
	mSkyboxShader->setUniformMat4f("gProjectionMat", projection);
	mSkyBox->mTexture->bind();
	mSkyBox->mMesh->render();

	glCullFace(GL_BACK);
}

void Renderer::renderScene(const glm::mat4& view, const glm::mat4& projection)
{
	glDepthMask(true);
	glDisable(GL_BLEND);
	glActiveTexture(GL_TEXTURE0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	
	renderSkybox(view, projection);

	glDepthFunc(GL_LESS);

	mAmbientShader->bind();
	mAmbientShader->setUniformMat4f("gViewMat", view);
	mAmbientShader->setUniformMat4f("gProjectionMat", projection);
	for (size_t i = 0; i < mRenderObjects.size(); i++)
	{
		mRenderObjects[i].mMaterial->mTexture->bind();
		mAmbientShader->setUniformMat4f("gModelMat", *mRenderObjects[i].mModelMat);
		mRenderObjects[i].mMesh->render();
	}

	/*mAmbientShaderTerrain->bind();
	mAmbientShader->setUniformMat4f("gViewMat", view);
	mAmbientShader->setUniformMat4f("gProjectionMat", projection);
	for (size_t i = 0; i < mRenderObjects.size(); i++)
	{
		mRenderObjects[i].mMaterial->mTexture->bind();
		mAmbientShader->setUniformMat4f("gModelMat", *mRenderObjects[i].mModelMat);
		mRenderObjects[i].mMesh->render();
	}*/

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	glDepthMask(false);
	glDepthFunc(GL_EQUAL);

	mDirectionalShader->bind();
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
	}

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

	mPointShader->bind();
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
	}

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
	
	assert(mWaterShader != NULL);
	mWaterShader->bind();
	mWaterShader->setUniformMat4f("gModelMat", glm::scale(glm::vec3(160, 160, 160)));
	mWaterShader->setUniformMat4f("gViewMat", view);
	mWaterShader->setUniformMat4f("gProjectionMat", projection);
	mWaterShader->setUniformFloat("gDuDvOffset", mWater->mDuDvOffset);
	mWaterShader->setUniformVec3f("gEyePos", mCamera->mPosition);
	mWaterShader->setUniformVec3f("gLightDir", mDirectionalLights[0].dir);
	mWater->render();
}
