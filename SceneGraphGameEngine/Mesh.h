#pragma once

#include "MeshData.h"

//Contains functionality to combine multiple mesh data and import meshes from assimp 
class Mesh
{
public:
	Mesh();
	Mesh(const std::string& filename);

	~Mesh();

	bool loadFromFile(const std::string& Filename);

	void render();

	bool InitFromScene(const aiScene* pScene, const std::string& Filename);
	void InitMesh(unsigned int Index, const aiMesh* paiMesh);
	bool InitMaterials(const aiScene* pScene, const std::string& Filename);
	void clear();

	std::vector<MeshDataTex> mEntries;
	std::vector<Texture2D*> mTextures;

private:
	#define INVALID_MATERIAL 0xFFFFFFFF
};
