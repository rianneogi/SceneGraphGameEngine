#pragma once

#include "ShaderProgram.h"

///Contains a Mesh and Texture together

class Model
{
public:
	Model();
	Model(Mesh* mesh, Texture* texture, ShaderProgram* shader);
	Model(const Model& other);
	~Model();

	void create(Mesh* mesh, Texture* texture, ShaderProgram* shader);
	void render();

	void move(const glm::vec3& vec);
	void setPosition(const glm::vec3& pos);
	void setMesh(Mesh* mesh);
	void setTexture(Texture* texture);
	void setShader(ShaderProgram* shader);
	void setModelMatrix(const glm::mat4& mat);
	void scale(float value);
	//bool rayTrace(Vector2i mousePos, const glm::mat4& projview, const Vector2i& screenDimensions);

	Mesh* mMesh;
	Texture* mTexture;
	ShaderProgram* mShader;
	glm::mat4 mModelMatrix;
};

