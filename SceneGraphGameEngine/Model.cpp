#include "Model.h"

Model::Model() : mMesh(NULL), mTexture(NULL), mShader(NULL), mModelMatrix(1.0f)
{
}

Model::Model(Mesh* mesh, Texture* texture, ShaderProgram* shader) : mMesh(mesh), mTexture(texture), mShader(shader), mModelMatrix(1.0f)
{
}

Model::Model(const Model& other)
{
	mMesh = other.mMesh;
	mTexture = other.mTexture;
	mShader = other.mShader;
	mModelMatrix = other.mModelMatrix;
}

Model::~Model()
{
}

void Model::render()
{
	assert(mMesh != NULL && mTexture != NULL);
	mTexture->bind();
	//mShader->bind();
	//mShader->setWorld(mModelMatrix);
	//mShader->updateWorld();
	/*glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);*/
	mMesh->render();
	/*glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);*/

	GLenum err = GL_NO_ERROR;
	if ((err = glGetError()) != GL_NO_ERROR)
	{
		printf("ERROR: Model render error %d", err);
	}
}

void Model::create(Mesh* mesh, Texture* texture, ShaderProgram* shader)
{
	mMesh = mesh;
	mTexture = texture;
	mShader = shader;
	mModelMatrix = glm::mat4(1.0f);
}

void Model::move(const glm::vec3& vec)
{
	mModelMatrix = glm::translate(mModelMatrix, vec);
}

void Model::setPosition(const glm::vec3& pos)
{
	mModelMatrix = glm::translate(glm::mat4(1.0f), pos);
}

void Model::scale(float value)
{
	mModelMatrix = glm::scale(mModelMatrix, glm::vec3(value, value, value));
}

void Model::setMesh(Mesh* mesh)
{
	mMesh = mesh;
}

void Model::setTexture(Texture* texture)
{
	mTexture = texture;
}

void Model::setShader(ShaderProgram* shader)
{
	mShader = shader;
}

void Model::setModelMatrix(const glm::mat4& mat)
{
	mModelMatrix = mat;
}

//bool Model::rayTrace(Vector2i mousePos, const glm::mat4& projview, const Vector2i& screenDimensions) //only works for end turn model
//{
//	Vector2f mousepixel;
//	mousepixel.x = mousePos.x / (screenDimensions.x / 2.f) - 1.f;
//	mousepixel.y = -(mousePos.y / (screenDimensions.y / 2.f) - 1.f);
//	glm::mat4 finalmat = projview*mModelMatrix;
//
//	std::vector<glm::vec4> newverts;
//	newverts.push_back(finalmat*glm::vec4(0.f, 0.f, -1.38f, 1));
//	newverts.push_back(finalmat*glm::vec4(5.f, 0.f, -1.38f, 1));
//	newverts.push_back(finalmat*glm::vec4(5.f, 0.f, 1.38f, 1));
//	newverts.push_back(finalmat*glm::vec4(-0.f, 0.f, 1.38f, 1));
//
//	for (int i = 0;i < 4;i++) //perspective divide
//	{
//		newverts[i].x /= newverts[i].w;
//		newverts[i].y /= newverts[i].w;
//		newverts[i].z /= newverts[i].w;
//	}
//
//	if (isPointInsidePolygon(newverts, mousepixel.x, mousepixel.y))
//	{
//		return true;
//	}
//	return false;
//}

