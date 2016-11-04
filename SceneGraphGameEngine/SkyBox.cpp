#include "SkyBox.h"

SkyBox::SkyBox() : mMesh(NULL), mTexture(NULL), mShader(NULL)
{
}

SkyBox::SkyBox(MeshDataTex* mesh, CubeMapTexture* texture) : mMesh(mesh), mTexture(texture)
{
}

SkyBox::SkyBox(const SkyBox& other)
{
	mMesh = other.mMesh;
	mTexture = other.mTexture;
	mShader = other.mShader;
}

SkyBox::~SkyBox()
{
}

//void SkyBox::render()
//{
//	assert(mMesh != NULL && mTexture != NULL);
//	mTexture->bind();
//	mMesh->render();
//}
