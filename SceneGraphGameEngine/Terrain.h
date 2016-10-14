#pragma once

#include "Timer.h"

class Terrain
{
public:
	MeshDataTex2DArray mMesh;
	TextureArray* mTexture;

	float* mHeightMap;
	int mLength;
	int mWidth;

	Terrain();
	~Terrain();

	glm::vec3 calculateNormal(int x, int z);

	void generate();
	void render();

	float getHeight(int x, int z);
};

