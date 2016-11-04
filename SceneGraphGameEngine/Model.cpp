#include "Model.h"

Model::Model() : mModelMatrix(1.0f)
{
}

Model::~Model()
{
}

//void Model::render()
//{
//	assert(mMesh != NULL && mTexture != NULL);
//	mTexture->bind();
//	mMesh->render();
//
//	GLenum err = GL_NO_ERROR;
//	if ((err = glGetError()) != GL_NO_ERROR)
//	{
//		printf("ERROR: Model render error %d", err);
//	}
//}

void Model::addMesh(MeshDataTex* mesh, Material* tex)
{
	mMeshes.push_back(mesh);
	mMaterials.push_back(tex);
}

void Model::addToRenderer(Renderer* renderer)
{
	for (size_t i = 0; i < mMeshes.size(); i++)
	{
		int shader = NONE;
		if (mMaterials[i]->mNormalMap != NULL)
		{
			shader |= NORMAL_MAP;
		}
		if (mMaterials[i]->mDisplacementMap != NULL)
		{
			shader |= PARALLAX_MAP;
		}
		if (mMaterials[i]->mSpecularMap != NULL)
		{
			shader |= SPECULAR_MAP;
		}

		renderer->addRenderObject(mMeshes[i], mMaterials[i], shader, &mModelMatrix);
	}
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

