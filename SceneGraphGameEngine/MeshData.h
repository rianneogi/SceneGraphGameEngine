#pragma once

#include "FrameBuffer.h"
#include "GraphicsTypedefs.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

struct Vertex
{
	glm::vec3 Pos;
	glm::vec2 UV;
	glm::vec3 Normal;
	glm::vec3 Tangent;
	Vertex(glm::vec3 p, glm::vec2 uv, glm::vec3 n, glm::vec3 t) : Pos(p), UV(uv), Normal(n), Tangent(t) {}
};

struct VertexTexArray
{
	glm::vec3 Pos;
	glm::vec3 UV;
	glm::vec3 Normal;
	glm::vec3 Tangent;
	VertexTexArray(glm::vec3 p, glm::vec3 uv, glm::vec3 n, glm::vec3 t) : Pos(p), UV(uv), Normal(n), Tangent(t) {}
};

struct VertexSolidColor
{
	glm::vec3 Pos;
	glm::vec3 Color;
	glm::vec3 Normal;
	VertexSolidColor(glm::vec3 p, glm::vec3 col, glm::vec3 n) : Pos(p), Color(col), Normal(n) {}
};

class MeshData
{
public:
	virtual void render() = 0;
};

//Contains mesh data
class MeshDataTex : public MeshData
{
public:
	MeshDataTex();
	MeshDataTex(std::string file);
	~MeshDataTex();

	bool loadFromFile(const std::string& Filename);

	bool InitFromScene(const aiScene* pScene, const std::string& Filename);
	void InitMesh(unsigned int Index, const aiMesh* paiMesh, std::vector<Vertex>& Vertices, std::vector<unsigned int>& Indices);
	//bool InitMaterials(const aiScene* pScene, const std::string& Filename);
	void clear();

	bool init(const std::vector<Vertex>& Vertices, const std::vector<unsigned int>& Indices);
	void render();
	//void clear();

	GLuint VB;
	GLuint IB;
	unsigned int NumIndices;
	unsigned int MaterialIndex;
};

//Contains data of an array textured mesh
class MeshDataTexArray : public MeshData
{
public:
	MeshDataTexArray();
	~MeshDataTexArray();

	bool init(const std::vector<VertexTexArray>& Vertices, const std::vector<unsigned int>& Indices);
	void render();
	void clear();

	GLuint VB;
	GLuint IB;
	unsigned int NumIndices;
	unsigned int MaterialIndex;
};

//Contains data of a solid-colored mesh
class MeshDataSolidColor : public MeshData
{
public:
	MeshDataSolidColor();
	~MeshDataSolidColor();

	bool init(const std::vector<VertexSolidColor>& Vertices, const std::vector<unsigned int>& Indices);
	void render();
	void clear();

	GLuint VB;
	GLuint IB;
	unsigned int NumIndices;
	unsigned int MaterialIndex;
};