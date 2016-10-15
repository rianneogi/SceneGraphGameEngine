#include "Water.h"

Water::Water()
{
	mModelMat = glm::mat4(1.0);
	mMesh = new Mesh("Resources\\Models\\plane.obj");

	mReflectionFBO.createFBO(SCREEN_WIDTH, SCREEN_HEIGHT);
	mReflectionFBO.createColorTextureAttachment();
	mReflectionFBO.createDepthBufferAttachment();

	mRefractionFBO.createFBO(SCREEN_WIDTH, SCREEN_HEIGHT);
	mReflectionFBO.createColorTextureAttachment();
	mReflectionFBO.createDepthTextureAttachment();
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

