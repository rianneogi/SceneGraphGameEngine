#include "MeshData.h"

MeshDataTex::MeshDataTex()
{
	VB = 0;
	IB = 0;
	NumIndices = 0;
	MaterialIndex = 0;
}

MeshDataTex::MeshDataTex(std::string file)
{
	VB = 0;
	IB = 0;
	NumIndices = 0;
	MaterialIndex = 0;

	loadFromFile(file);
}

MeshDataTex::~MeshDataTex()
{
	clear();
}

void MeshDataTex::clear()
{
	// Disable the two vertex array attributes.
	/*glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);*/

	// Release the vertex buffer.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &VB);

	//Index Buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &IB);
}

bool MeshDataTex::loadFromFile(const std::string& Filename)
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

bool MeshDataTex::InitFromScene(const aiScene* pScene, const std::string& Filename)
{
	//mEntries.resize(pScene->mNumMeshes);
	//mTextures.resize(pScene->mNumMaterials);

	// Initialize the meshes in the scene one by one
	std::vector<Vertex> Vertices;
	std::vector<unsigned int> Indices;
	for (unsigned int i = 0; i < pScene->mNumMeshes; i++)
	{
		const aiMesh* paiMesh = pScene->mMeshes[i];
		InitMesh(i, paiMesh, Vertices, Indices);
	}
	init(Vertices, Indices);
	return true;
	//return InitMaterials(pScene, Filename);
}

void MeshDataTex::InitMesh(unsigned int Index, const aiMesh* paiMesh, std::vector<Vertex>& Vertices, std::vector<unsigned int>& Indices)
{
	//mEntries[Index].MaterialIndex = paiMesh->mMaterialIndex;
	const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

	for (unsigned int i = 0; i < paiMesh->mNumVertices; i++)
	{
		const aiVector3D* pPos = &(paiMesh->mVertices[i]);
		const aiVector3D* pNormal = paiMesh->HasNormals() ? &(paiMesh->mNormals[i]) : &Zero3D;
		const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;
		const aiVector3D* pTangent = paiMesh->HasTangentsAndBitangents() ? &(paiMesh->mTangents[i]) : &Zero3D;

		Vertex v(glm::vec3(pPos->x, pPos->y, pPos->z),
			glm::vec2(pTexCoord->x, pTexCoord->y),
			glm::vec3(pNormal->x, pNormal->y, pNormal->z),
			glm::vec3(pTangent->x, pTangent->y, pTangent->z));

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
}

//bool MeshDataTex::InitMaterials(const aiScene* pScene, const std::string& Filename)
//{
//	bool Ret = true;
//	for (unsigned int i = 0; i < pScene->mNumMaterials; i++)
//	{
//		const aiMaterial* pMaterial = pScene->mMaterials[i];
//
//		mTextures[i] = NULL;
//		if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0)
//		{
//			aiString Path;
//
//			if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
//				std::string FullPath = Path.data;
//				mTextures[i] = new Texture2D();
//				if (!mTextures[i]->loadFromFile(FullPath.c_str()))
//				{
//					printf("Error loading texture '%s'\n", FullPath.c_str());
//					delete mTextures[i];
//					mTextures[i] = NULL;
//					Ret = false;
//				}
//
//				/*if (!m_Textures[i]->Load()) {
//				printf("Error loading texture '%s'\n", FullPath.c_str());
//				delete m_Textures[i];
//				m_Textures[i] = NULL;
//				Ret = false;
//				}*/
//			}
//		}
//
//		if (!mTextures[i])
//		{
//			printf("ERROR: cant load Texture, no texture present\n");
//			Ret = false;
//			//m_Textures[i] = new Texture(GL_TEXTURE_2D, "../Content/white.png");
//			//Ret = m_Textures[i]->Load();
//		}
//	}
//
//	return Ret;
//}

bool MeshDataTex::init(const std::vector<Vertex>& Vertices, const std::vector<unsigned int>& Indices)
{
	clear();

	glGenBuffers(1, &VB);
	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

	// Generate an ID for the index buffer.
	glGenBuffers(1, &IB);

	// Bind the index buffer and load the index data into it.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(unsigned int), &Indices[0], GL_STATIC_DRAW);

	NumIndices = Indices.size();
	printf("Created Mesh, VB: %d, IB: %d\n", (int)VB, (int)IB);
	return true;
}

void MeshDataTex::render()
{
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)offsetof(Vertex, Pos));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)offsetof(Vertex, UV));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)offsetof(Vertex, Normal));
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)offsetof(Vertex, Tangent));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

	//const unsigned int MaterialIndex = m_Entries[i].MaterialIndex;

	//if (MaterialIndex < m_Textures.size() && m_Textures[MaterialIndex]) {
	//	//m_Textures[MaterialIndex]->bind(GL_TEXTURE0);
	//	//m_Textures[MaterialIndex]->bind();
	//}
	debugOpengl("PRE-DRAW ERROR");
	glDrawElements(GL_TRIANGLES, NumIndices, GL_UNSIGNED_INT, 0);

	debugOpengl("DRAW ERROR");

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
}

MeshDataTexArray::MeshDataTexArray()
{
	VB = 0;
	IB = 0;
	NumIndices = 0;
	MaterialIndex = 0;
}

MeshDataTexArray::~MeshDataTexArray()
{
	clear();
}

void MeshDataTexArray::clear()
{
	// Disable the two vertex array attributes.
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	// Release the vertex buffer.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &VB);

	//Index Buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &IB);
}

bool MeshDataTexArray::init(const std::vector<VertexTexArray>& Vertices, const std::vector<unsigned int>& Indices)
{
	clear();

	glGenBuffers(1, &VB);
	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexTexArray)*Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

	// Generate an ID for the index buffer.
	glGenBuffers(1, &IB);

	// Bind the index buffer and load the index data into it.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(unsigned int), &Indices[0], GL_STATIC_DRAW);

	NumIndices = Indices.size();
	return true;
}

void MeshDataTexArray::render()
{
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	//glEnableVertexAttribArray(3);

	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexTexArray), (const GLvoid*)offsetof(VertexTexArray, Pos));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexTexArray), (const GLvoid*)offsetof(VertexTexArray, UV));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexTexArray), (const GLvoid*)offsetof(VertexTexArray, Normal));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
	//const unsigned int MaterialIndex = m_Entries[i].MaterialIndex;

	//if (MaterialIndex < m_Textures.size() && m_Textures[MaterialIndex]) {
	//	//m_Textures[MaterialIndex]->bind(GL_TEXTURE0);
	//	//m_Textures[MaterialIndex]->bind();
	//}
	glDrawElements(GL_TRIANGLES, NumIndices, GL_UNSIGNED_INT, 0);

	debugOpengl("DRAW ERROR tex array");

	//debugOpengl("mesh entry 2d array draw");
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	//glDisableVertexAttribArray(3);
}

MeshDataSolidColor::MeshDataSolidColor()
{
	VB = 0;
	IB = 0;
	NumIndices = 0;
	MaterialIndex = 0;
}

MeshDataSolidColor::~MeshDataSolidColor()
{
	clear();
}

bool MeshDataSolidColor::init(const std::vector<VertexSolidColor>& Vertices, const std::vector<unsigned int>& Indices)
{
	clear();

	glGenBuffers(1, &VB);
	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexSolidColor)*Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

	// Generate an ID for the index buffer.
	glGenBuffers(1, &IB);

	// Bind the index buffer and load the index data into it.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(unsigned int), &Indices[0], GL_STATIC_DRAW);

	NumIndices = Indices.size();
	return true;
}

void MeshDataSolidColor::render()
{
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	//glEnableVertexAttribArray(3);

	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexSolidColor), (const GLvoid*)offsetof(VertexSolidColor, Pos));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexSolidColor), (const GLvoid*)offsetof(VertexSolidColor, Color));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexSolidColor), (const GLvoid*)offsetof(VertexSolidColor, Normal));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

	//const unsigned int MaterialIndex = m_Entries[i].MaterialIndex;

	//if (MaterialIndex < m_Textures.size() && m_Textures[MaterialIndex]) {
	//	//m_Textures[MaterialIndex]->bind(GL_TEXTURE0);
	//	//m_Textures[MaterialIndex]->bind();
	//}
	glDrawElements(GL_TRIANGLES, NumIndices, GL_UNSIGNED_INT, 0);

	debugOpengl("DRAW ERROR solid color");

	//debugOpengl("mesh entry 2d array draw");
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	//glDisableVertexAttribArray(3);
}

void MeshDataSolidColor::clear()
{
	// Disable the two vertex array attributes.
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	// Release the vertex buffer.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &VB);

	//Index Buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &IB);
}