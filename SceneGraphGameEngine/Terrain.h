#pragma once

#include "Model.h"

extern float CONST_BLOCKSIZE;
extern int CONST_CHUNKSIZE;

class Terrain
{
public:
	MeshDataTexArray* mMesh;
	Material* mMaterial;

	float* mHeightMap;
	Vector2i mChunkPos;
	int mLength;
	int mWidth;

	glm::mat4 mModelMat;

	Terrain();
	Terrain(int i, int j, Material* mat);
	~Terrain();

	void generate();
	void render();

	void addToRenderer(Renderer* renderer);

	glm::vec3 calculateNormal(int x, int z);
	float getHeight(int x, int z);
};

