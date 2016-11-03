#pragma once

#include "Model.h"

extern float CONST_BLOCKSIZE;
extern int CONST_CHUNKSIZE;

class Terrain
{
public:
	MeshDataTexArray* mMesh;

	float* mHeightMap;
	Vector2i mChunkPos;
	int mLength;
	int mWidth;

	Terrain();
	Terrain(int i, int j);
	~Terrain();

	glm::vec3 calculateNormal(int x, int z);

	void generate();
	void render();

	float getHeight(int x, int z);
};

