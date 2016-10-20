#include "Water.h"

Water::Water(Mesh* mesh)
{
	mModelMat = glm::mat4(1.0);
	mMesh = mesh;

	mReflectionFBO.initFBO(SCREEN_WIDTH, SCREEN_HEIGHT, GL_COLOR_ATTACHMENT0);
	mReflectionFBO.createColorTextureAttachment();
	mReflectionFBO.createDepthBufferAttachment();
	mReflectionFBO.checkStatus();
	mReflectionFBO.unbind();

	mRefractionFBO.initFBO(SCREEN_WIDTH, SCREEN_HEIGHT, GL_COLOR_ATTACHMENT0);
	mRefractionFBO.createColorTextureAttachment();
	mRefractionFBO.createDepthTextureAttachment();
	mRefractionFBO.checkStatus();
	mRefractionFBO.unbind();

	mWaveSpeed = 0.000025f;
	mWaveStrength = 0.04f;
	mDuDvOffset = 0.f;
	mWaveHeight = 0.05f;
	mSpecularFactor = 0.5f;
	mShineDamper = 20.f;
}

Water::~Water()
{
}

void Water::update(int deltaTime)
{
	mDuDvOffset += deltaTime*mWaveSpeed;
	if (mDuDvOffset > 1.0f)
	{
		mDuDvOffset -= 1.0f;
	}
}

void Water::render()
{
	mMesh->render();
}

void Water::bindReflection()
{
	mReflectionFBO.bind();
}

void Water::bindRefraction()
{
	mRefractionFBO.bind();
}

