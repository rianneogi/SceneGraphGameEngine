#include "Terrain.h"

float CONST_BLOCKSIZE = 1.0f;
int CONST_CHUNKSIZE = 128;

Terrain::Terrain() : mMesh(NULL), mMaterial(NULL), mChunkPos(0,0), mModelMat(1.0)
{
}

Terrain::Terrain(int i, int j, Material* mat) : mMesh(NULL), mMaterial(mat), mChunkPos(i, j), mModelMat(1.0)
{
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
	return mHeightMap[x*(mWidth+2) + z];
}

float perlinOctaves(float i, float j)
{
	float res = 0;
	//res += 4 * glm::perlin(glm::vec2(i / 20.0, j / 20.0));
	res += 8 * glm::perlin(glm::vec2(i / 40.0, j / 40.0));
	res += 16 * glm::perlin(glm::vec2(i / 80.0, j / 80.0));
	res += 32 * glm::perlin(glm::vec2(i / 160.0, j / 160.0));
	res += 64 * glm::perlin(glm::vec2(i / 320.0, j / 320.0));
	//res += 64 * glm::perlin(glm::vec2(i / 640.0, j / 640.0));
	return res;
}

void Terrain::generate()
{
	mMesh = new MeshDataTexArray();
	std::vector<VertexTexArray> Vertices;
	std::vector<unsigned int> Indices;
	unsigned int curr_ind = 0;

	mWidth = CONST_CHUNKSIZE;
	mLength = CONST_CHUNKSIZE;
	mHeightMap = new float[(mWidth+2)*(mLength+2)];
	for (int i = 0;i < mLength+2;i++)
	{
		for (int j = 0;j < mWidth+2;j++)
		{
			mHeightMap[i * (mWidth+2) + j] = perlinOctaves(mChunkPos.x*CONST_CHUNKSIZE+i, mChunkPos.y*CONST_CHUNKSIZE+j);
		}
	}

	for (int i = 0;i < mLength;i++)
	{
		for (int j = 0;j < mWidth;j++)
		{
			int tex = 0;
			/*if (j > 32)
			{
				tex = 3;
			}*/
			Vertices.push_back(VertexTexArray(glm::vec3(mChunkPos.x*CONST_BLOCKSIZE*CONST_CHUNKSIZE,0, mChunkPos.y*CONST_BLOCKSIZE*CONST_CHUNKSIZE) + 
				glm::vec3(i, getHeight(i, j), j), glm::vec3(i,j,tex), calculateNormal(i+1, j+1), glm::vec3(1,0,0)));
		}
	}

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

void Terrain::addToRenderer(Renderer* renderer)
{
	renderer->addRenderObject(mMesh, mMaterial, MULTI_TEXTURE, &mModelMat);
}
