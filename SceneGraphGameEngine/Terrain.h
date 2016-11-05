#pragma once

#include "Model.h"

extern float CONST_BLOCKSIZE;
extern int CONST_CHUNKSIZE;

class Terrain
{
public:
	MeshDataTexArray* mMesh;
	Material* mMaterial;

	std::vector<VertexTexArray> mVertices;
	std::vector<unsigned int> mIndices;

	float* mHeightMap;
	Vector2i mChunkPos;
	int mLength;
	int mWidth;

	glm::mat4 mModelMat;

	Terrain();
	Terrain(int i, int j, Material* mat);
	~Terrain();

	void addQuad(glm::vec3 start, glm::vec2 size, glm::vec3 normal, glm::vec3 tangent, glm::vec3 bitangent, unsigned int texture);

	void generate();
	void generateMesh();
	void render();

	void flatten(int x, int y, int l, int w, int height);

	void addToRenderer(Renderer* renderer);

	glm::vec3 calculateNormal(int x, int z);
	float getHeight(int x, int z);
};

