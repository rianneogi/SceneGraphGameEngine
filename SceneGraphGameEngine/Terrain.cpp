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
	glm::vec3 normal = glm::vec3(getHeight(x - 1, z) - getHeight(x + 1, z), 2, getHeight(x, z - 1) - getHeight(x, z + 1));
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
	//res += 8 * glm::perlin(glm::vec2(i / 40.0, j / 40.0));
	res += 16 * glm::perlin(glm::vec2(i / 80.0, j / 80.0));
	res += 32 * glm::perlin(glm::vec2(i / 160.0, j / 160.0));
	res += 64 * glm::perlin(glm::vec2(i / 320.0, j / 320.0));
	//res += 64 * glm::perlin(glm::vec2(i / 640.0, j / 640.0));
	return res;
}

void Terrain::addQuad(glm::vec3 start, glm::vec2 size, glm::vec3 normal, glm::vec3 tangent, glm::vec3 bitangent, unsigned int texture)
{
	unsigned int curr_ind = mVertices.size();

	mVertices.push_back(VertexTexArray(glm::vec3(mChunkPos.x*CONST_BLOCKSIZE*CONST_CHUNKSIZE, 0, mChunkPos.y*CONST_BLOCKSIZE*CONST_CHUNKSIZE) +
		start, glm::vec3(0, 0, texture), normal, tangent));
	mVertices.push_back(VertexTexArray(glm::vec3(mChunkPos.x*CONST_BLOCKSIZE*CONST_CHUNKSIZE, 0, mChunkPos.y*CONST_BLOCKSIZE*CONST_CHUNKSIZE) +
		start + tangent*size.x, glm::vec3(size.x, 0, texture), normal, glm::rotateY(glm::vec3(0, 0, -1), glm::pi<float>() / 2.f)));
	mVertices.push_back(VertexTexArray(glm::vec3(mChunkPos.x*CONST_BLOCKSIZE*CONST_CHUNKSIZE, 0, mChunkPos.y*CONST_BLOCKSIZE*CONST_CHUNKSIZE) +
		start + bitangent*size.y, glm::vec3(0, size.y, texture), normal, tangent));
	mVertices.push_back(VertexTexArray(glm::vec3(mChunkPos.x*CONST_BLOCKSIZE*CONST_CHUNKSIZE, 0, mChunkPos.y*CONST_BLOCKSIZE*CONST_CHUNKSIZE) +
		start + tangent*size.x + bitangent*size.y, glm::vec3(size.x, size.y, texture), normal, tangent));

	mIndices.push_back(curr_ind);
	mIndices.push_back(curr_ind + 1);
	mIndices.push_back(curr_ind + 2);
	mIndices.push_back(curr_ind + 3);
	mIndices.push_back(curr_ind + 2);
	mIndices.push_back(curr_ind + 1);
}

void Terrain::generate()
{
	mMesh = new MeshDataTexArray();
	/*std::vector<VertexTexArray> Vertices;
	std::vector<unsigned int> Indices;*/
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

	flatten(5, 5, 50, 50, getHeight(5,5));

	for (int i = 0;i < mLength;i++)
	{
		for (int j = 0;j < mWidth;j++)
		{
			int tex = 3;
			/*if (j > 32)
			{
				tex = 3;
			}*/
			glm::vec3 normal = calculateNormal(i + 1, j + 1);
			mVertices.push_back(VertexTexArray(glm::vec3(mChunkPos.x*CONST_BLOCKSIZE*CONST_CHUNKSIZE,0, mChunkPos.y*CONST_BLOCKSIZE*CONST_CHUNKSIZE) + 
				glm::vec3(i, getHeight(i, j), j), glm::vec3(i,j,tex), normal, glm::rotateZ(normal, glm::pi<float>()/2.f)));
		}
	}

	for (int i = 0;i < mLength-1;i++)
	{
		for (int j = 0;j < mWidth-1;j++)
		{
			curr_ind = i*mWidth + j;
			mIndices.push_back(curr_ind);
			mIndices.push_back(curr_ind+1);
			mIndices.push_back(curr_ind + mWidth);
			mIndices.push_back(curr_ind + mWidth + 1);
			mIndices.push_back(curr_ind + mWidth);
			mIndices.push_back(curr_ind+1);
		}
	}

	addQuad(glm::vec3(10, getHeight(5, 5) + 45, 10), glm::vec2(45, 45), glm::vec3(0, 0, -1), glm::vec3(1, 0, 0), glm::vec3(0, -1, 0), 0);
	addQuad(glm::vec3(55, getHeight(5, 5) + 45, 10), glm::vec2(45, 45), glm::vec3(-1, 0, 0), glm::vec3(0, 0, 1), glm::vec3(0, -1, 0), 0);
	addQuad(glm::vec3(10, getHeight(5, 5) + 45, 55), glm::vec2(45, 45), glm::vec3(1, 0, 0), glm::vec3(0, 0, -1), glm::vec3(0, -1, 0), 0);
	addQuad(glm::vec3(55, getHeight(5, 5) + 45, 55), glm::vec2(45, 45), glm::vec3(0, 0, 1), glm::vec3(-1, 0, 0), glm::vec3(0, -1, 0), 0);
	addQuad(glm::vec3(10, getHeight(5, 5) + 45, 10), glm::vec2(45, 45), glm::vec3(0, 1, 0), glm::vec3(0, 0, 1), glm::vec3(1, 0, 0), 0);

	mMesh->init(mVertices, mIndices);
}

void Terrain::generateMesh()
{
	mMesh->init(mVertices, mIndices);
}

void Terrain::render()
{
	mMesh->render();
}

void Terrain::flatten(int x, int y, int l, int w, int height)
{
	for (int i = x; i < x + l; i++)
	{
		for (int j = y; j < y + w; j++)
		{
			mHeightMap[i * (mWidth + 2) + j] = height;
		}
	}
}

void Terrain::addToRenderer(Renderer* renderer)
{
	renderer->addRenderObject(mMesh, mMaterial, MULTI_TEXTURE, &mModelMat);
}
