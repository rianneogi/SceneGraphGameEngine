#pragma once

#include "Camera.h"

class SkyBox
{
public:
	MeshDataTex* mMesh;
	CubeMapTexture* mTexture;
	ShaderProgram* mShader;

	SkyBox();
	SkyBox(const SkyBox& other);
	SkyBox(MeshDataTex* mesh, CubeMapTexture* texture);
	~SkyBox();

	//void render();
};

