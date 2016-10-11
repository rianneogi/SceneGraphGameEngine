#pragma once

#include "MeshData.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

//Contains functionality to combine multiple mesh data and import meshes from assimp 
class Mesh
{
public:
	Mesh();

	~Mesh();

	bool loadFromFile(const std::string& Filename);

	void render();

	bool InitFromScene(const aiScene* pScene, const std::string& Filename);
	void InitMesh(unsigned int Index, const aiMesh* paiMesh);
	bool InitMaterials(const aiScene* pScene, const std::string& Filename);
	void clear();

	std::vector<MeshData> mEntries;
	std::vector<Texture*> mTextures;

private:
	#define INVALID_MATERIAL 0xFFFFFFFF
};
