#include "Water.h"

Water::Water(Mesh* mesh)
{
	mModelMat = glm::mat4(1.0);
	mMesh = mesh;

	mReflectionFBO.createFBO(SCREEN_WIDTH, SCREEN_HEIGHT);
	mReflectionFBO.createColorTextureAttachment();
	mReflectionFBO.createDepthBufferAttachment();
	mReflectionFBO.checkStatus();
	mReflectionFBO.unbind();

	mRefractionFBO.createFBO(SCREEN_WIDTH, SCREEN_HEIGHT);
	mRefractionFBO.createColorTextureAttachment();
	mRefractionFBO.createDepthTextureAttachment();
	mRefractionFBO.checkStatus();
	mRefractionFBO.unbind();

	mWaveSpeed = 0.000025f;
	mDuDvOffset = 0.f;
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

