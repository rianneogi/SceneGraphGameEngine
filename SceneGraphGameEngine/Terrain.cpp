#include "Terrain.h"

Terrain::Terrain()
{
	mTexture = NULL;
	mMesh = NULL;
}

Terrain::~Terrain()
{
	delete mMesh;
}

glm::vec3 Terrain::calculateNormal(int x, int z)
{
	glm::vec3 normal = glm::vec3(getHeight(x - 1, z) - getHeight(x + 1, z), 2, getHeight(x, z-1) - getHeight(x, z+1));
	return glm::normalize(normal);
}

float Terrain::getHeight(int x, int z)
{
	return mHeightMap[x*mWidth + z];
}

float perlinOctaves(float i, float j)
{
	float res = 2 * glm::perlin(glm::vec2(i / 20.0, j / 20.0));
	res += 4 * glm::perlin(glm::vec2(i / 40.0, j / 40.0));
	res += 8 * glm::perlin(glm::vec2(i / 80.0, j / 80.0));
	res += 16 * glm::perlin(glm::vec2(i / 160.0, j / 160.0));
	return res;
}

void Terrain::generate()
{
	mMesh = new MeshDataTexArray();
	std::vector<VertexTexArray> Vertices;
	std::vector<unsigned int> Indices;
	unsigned int curr_ind = 0;

	mWidth = 64;
	mLength = 64;
	mHeightMap = new float[(mWidth+2)*(mLength+2)];
	for (int i = 0;i < mLength+2;i++)
	{
		for (int j = 0;j < mWidth+2;j++)
		{
			mHeightMap[i * mWidth + j] = perlinOctaves(i,j);
		}
	}

	/*glm::vec3* colors = new glm::vec3[4];
	colors[0] = glm::vec3(196 / 255.0, 212 / 255.0, 170 / 255.0);
	colors[1] = glm::vec3(221/255.0, 221/255.0, 187/255.0);
	colors[2] = glm::vec3(233/255.0, 221/255.0, 199/255.0);
	colors[3] = glm::vec3(156/255.0, 187/255.0, 169/255.0);*/

	for (int i = 0;i < mLength;i++)
	{
		for (int j = 0;j < mWidth;j++)
		{
			int tex = 0;
			if (j > 32)
			{
				tex = 2;
			}
			Vertices.push_back(VertexTexArray(glm::vec3(i, getHeight(i, j), j), glm::vec3(i,j,tex), calculateNormal(i+1, j+1)));
		}
	}

	//delete[] colors;

	for (int i = 0;i < mLength-1;i++)
	{
		for (int j = 0;j < mWidth-1;j++)
		{
			curr_ind = i*mWidth + j;
			Indices.push_back(curr_ind);
			Indices.push_back(curr_ind+1);
			Indices.push_back(curr_ind + mWidth);
			Indices.push_back(curr_ind + mWidth + 1);
			Indices.push_back(curr_ind + mWidth);
			Indices.push_back(curr_ind+1);
		}
	}

	mMesh->init(Vertices, Indices);
}

void Terrain::render()
{
	mMesh->render();
}
