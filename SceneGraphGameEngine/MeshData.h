#pragma once

#include "FrameBuffer.h"
#include "GraphicsTypedefs.h"

struct Vertex
{
	glm::vec3 Pos;
	glm::vec2 UV;
	glm::vec3 Normal;
	Vertex(glm::vec3 p, glm::vec2 uv, glm::vec3 n) : Pos(p), UV(uv), Normal(n) {}
};

struct VertexTex2DArray
{
	glm::vec3 Pos;
	glm::vec3 UV;
	glm::vec3 Normal;
	VertexTex2DArray(glm::vec3 p, glm::vec3 uv, glm::vec3 n) : Pos(p), UV(uv), Normal(n) {}
};

//Contains mesh data
class MeshData
{
public:
	MeshData();
	~MeshData();

	bool init(const std::vector<Vertex>& Vertices, const std::vector<unsigned int>& Indices);
	void render();
	void clear();

	GLuint VB;
	GLuint IB;
	unsigned int NumIndices;
	unsigned int MaterialIndex;
};

//Contains data of a 3d textured mesh
class MeshDataTex2DArray
{
public:
	MeshDataTex2DArray();
	~MeshDataTex2DArray();

	bool init(const std::vector<VertexTex2DArray>& Vertices, const std::vector<unsigned int>& Indices);
	void render();
	void clear();

	GLuint VB;
	GLuint IB;
	unsigned int NumIndices;
	unsigned int MaterialIndex;
};
