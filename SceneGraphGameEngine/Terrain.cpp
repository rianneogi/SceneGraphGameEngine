#include "Terrain.h"

Terrain::Terrain()
{
	mTexture = NULL;
	//mMesh = NULL;
}

Terrain::~Terrain()
{
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

void Terrain::generate()
{
	std::vector<VertexTex2DArray> Vertices;
	std::vector<unsigned int> Indices;
	unsigned int curr_ind = 0;

	mWidth = 1000;
	mLength = 1000;
	mHeightMap = new float[(mWidth+2)*(mLength+2)];
	for (int i = 0;i < mLength+2;i++)
	{
		for (int j = 0;j < mWidth+2;j++)
		{
			mHeightMap[i * mWidth + j] = 5 * glm::perlin(glm::vec2(i / 10.0, j / 10.0));
		}
	}

	for (int i = 0;i < mLength;i++)
	{
		for (int j = 0;j < mWidth;j++)
		{
			int tex = rand() % 4;
			Vertices.push_back(VertexTex2DArray(glm::vec3(i, getHeight(i, j), j), glm::vec3(i, j, tex), calculateNormal(i+1, j+1)));
		}
	}

	for (int i = 0;i < mLength-1;i++)
	{
		for (int j = 0;j < mWidth-1;j++)
		{
			curr_ind = i*mWidth + j;
			Indices.push_back(curr_ind);
			Indices.push_back(curr_ind+1);
			Indices.push_back(curr_ind+mWidth);
			Indices.push_back(curr_ind + mWidth + 1);
			Indices.push_back(curr_ind + mWidth);
			Indices.push_back(curr_ind+1);

			//curr_ind += 1;
		}
	}

	//post-processing
	/*for (int i = 0;i < Vertices.size();i++)
	{
		for (int j = i + 1;j < Vertices.size();j++)
		{
			if (Vertices[i].Pos == Vertices[j].Pos && Vertices[i].UV == Vertices[j].UV && Vertices[i].Normal == Vertices[j].Normal)
			{
				for (int k = 0;k < Indices.size();k++)
				{
					if (Indices[k] == j)
					{
						Indices[k] = i;
					}
				}
				Vertices.erase(Vertices.begin() + j);
			}
		}
	}*/

	mMesh.init(Vertices, Indices);
}

void Terrain::render()
{
	mMesh.render();
}
