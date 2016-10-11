#include "Mesh.h"

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
	clear();
}

void Mesh::clear()
{
	mEntries.clear();
	/*unsigned int size = m_Textures.size();
	while (!m_Textures.empty())
	{
		delete m_Textures[size - 1];
		m_Textures.pop_back();
	}*/
}

bool Mesh::loadFromFile(const std::string& Filename)
{
	// Release the previously loaded mesh (if it exists)
    clear();

	bool Ret = true;
	Assimp::Importer Importer;

	const aiScene* pScene = Importer.ReadFile(Filename.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs 
		| aiProcess_CalcTangentSpace | aiProcess_JoinIdenticalVertices | aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph 
		| aiProcess_ValidateDataStructure
		);
	//printf("%s", Importer.GetErrorString());
	if (pScene) {
		Ret = InitFromScene(pScene, Filename);
	}
	else {
		printf("Error parsing '%s': '%s'\n", Filename.c_str(), Importer.GetErrorString());
		Ret = false;
	}
	return Ret;
}

bool Mesh::InitFromScene(const aiScene* pScene, const std::string& Filename)
{
	mEntries.resize(pScene->mNumMeshes);
	mTextures.resize(pScene->mNumMaterials);

	// Initialize the meshes in the scene one by one
	for (unsigned int i = 0; i < mEntries.size(); i++) 
	{
		const aiMesh* paiMesh = pScene->mMeshes[i];
		InitMesh(i, paiMesh);
	}
	return true;
	//return InitMaterials(pScene, Filename);
}

void Mesh::InitMesh(unsigned int Index, const aiMesh* paiMesh)
{
	mEntries[Index].MaterialIndex = paiMesh->mMaterialIndex;

	std::vector<Vertex> Vertices;
	std::vector<unsigned int> Indices;

	const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

	for (unsigned int i = 0; i < paiMesh->mNumVertices; i++) 
	{
		const aiVector3D* pPos = &(paiMesh->mVertices[i]);
		const aiVector3D* pNormal = paiMesh->HasNormals() ? &(paiMesh->mNormals[i]) : &Zero3D;
		const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

		Vertex v(Vector3f(pPos->x, pPos->y, pPos->z),
			Vector2f(pTexCoord->x, pTexCoord->y),
			Vector3f(pNormal->x, pNormal->y, pNormal->z));

		Vertices.push_back(v);
	}

	for (unsigned int i = 0; i < paiMesh->mNumFaces; i++) 
	{
		const aiFace& Face = paiMesh->mFaces[i];
		assert(Face.mNumIndices == 3);
		Indices.push_back(Face.mIndices[0]);
		Indices.push_back(Face.mIndices[1]);
		Indices.push_back(Face.mIndices[2]);
	}

	mEntries[Index].init(Vertices, Indices);
}

bool Mesh::InitMaterials(const aiScene* pScene, const std::string& Filename)
{
	bool Ret = true;
	for (unsigned int i = 0; i < pScene->mNumMaterials; i++) 
	{
		const aiMaterial* pMaterial = pScene->mMaterials[i];

		mTextures[i] = NULL;
		if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) 
		{
			aiString Path;

			if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
				std::string FullPath = Path.data;
				mTextures[i] = new Texture();
				if (!mTextures[i]->loadFromFile(FullPath.c_str()))
				{
					printf("Error loading texture '%s'\n", FullPath.c_str());
					delete mTextures[i];
					mTextures[i] = NULL;
					Ret = false;
				}

				/*if (!m_Textures[i]->Load()) {
					printf("Error loading texture '%s'\n", FullPath.c_str());
					delete m_Textures[i];
					m_Textures[i] = NULL;
					Ret = false;
				}*/
			}
		}

		if (!mTextures[i]) 
		{
			printf("ERROR: cant load Texture, no texture present\n");
			Ret = false;
			//m_Textures[i] = new Texture(GL_TEXTURE_2D, "../Content/white.png");
			//Ret = m_Textures[i]->Load();
		}
	}

	return Ret;
}

void Mesh::render()
{
	/*glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);*/
	//printf("entries size: %d\n", mEntries.size());
	for (size_t i = 0; i < mEntries.size(); i++) 
	{
		mEntries[i].render();
	}
	/*glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);*/
}
