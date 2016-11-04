#pragma once

#include "Renderer.h"

///Contains a Mesh and Texture together

class Model
{
public:
	Model();
	~Model();

	void addMesh(MeshDataTex* mesh, Material* tex);
	void addToRenderer(Renderer* renderer);

	void move(const glm::vec3& vec);
	void setPosition(const glm::vec3& pos);
	void scale(float value);
	//bool rayTrace(Vector2i mousePos, const glm::mat4& projview, const Vector2i& screenDimensions);

	std::vector<MeshDataTex*> mMeshes;
	std::vector<Material*> mMaterials;
	glm::mat4 mModelMatrix;
};

