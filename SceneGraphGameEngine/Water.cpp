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
}

Water::~Water()
{
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

