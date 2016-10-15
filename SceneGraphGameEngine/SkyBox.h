#pragma once

#include "Camera.h"

class SkyBox
{
public:
	SkyBox();
	SkyBox(const SkyBox& other);
	SkyBox(Mesh* mesh, CubeMapTexture* texture);
	~SkyBox();

	void render();

	Mesh* mMesh;
	CubeMapTexture* mTexture;
	glm::mat4 mModelMatrix;
};

