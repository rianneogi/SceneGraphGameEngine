#pragma once

#include "Texture.h"
#include "GraphicsTypedefs.h"

struct Vertex
{
	Vector3f Pos;
	Vector2f UV;
	Vector3f Normal;
	Vertex(Vector3f p, Vector2f uv, Vector3f n) : Pos(p), UV(uv), Normal(n) {}
};

struct VertexTex3D
{
	Vector3f Pos;
	Vector3f UV;
	Vector3f Normal;
	VertexTex3D(Vector3f p, Vector3f uv, Vector3f n) : Pos(p), UV(uv), Normal(n) {}
	VertexTex3D(Vector3f p, Vector2f uv, float material, Vector3f n) : Pos(p), UV(uv, material), Normal(n) {}
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
class MeshDataTexture3D
{
public:
	MeshDataTexture3D();
	~MeshDataTexture3D();

	bool Init(const std::vector<VertexTex3D>& Vertices, const std::vector<unsigned int>& Indices);
	void render();
	void clear();

	GLuint VB;
	GLuint IB;
	unsigned int NumIndices;
	unsigned int MaterialIndex;
};
