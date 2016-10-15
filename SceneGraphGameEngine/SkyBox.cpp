#include "SkyBox.h"

SkyBox::SkyBox() : mMesh(NULL), mTexture(NULL)
{
}

SkyBox::SkyBox(Mesh* mesh, CubeMapTexture* texture) : mMesh(mesh), mTexture(texture)
{
}

SkyBox::SkyBox(const SkyBox& other)
{
	mMesh = other.mMesh;
	mTexture = other.mTexture;
	mModelMatrix = other.mModelMatrix;
}

SkyBox::~SkyBox()
{
}

void SkyBox::render()
{
	assert(mMesh != NULL && mTexture != NULL);
	mTexture->bind();
	mMesh->render();
}
