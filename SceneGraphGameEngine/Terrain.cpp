#include "Terrain.h"

Terrain::Terrain()
{
}

Terrain::~Terrain()
{
}

void Terrain::generate()
{
	std::vector<VertexTex3D> Vertices;
	std::vector<unsigned int> Indices;
	unsigned int curr_ind = 0;

	for (int i = 0;i < 100;i++)
	{
		for (int j = 0;j < 100;j++)
		{
			printf("%f\n", glm::perlin(glm::vec2(i/ 1.0, j / 100.0)), j );
			Vertices.push_back(VertexTex3D(Vector3f(i, 5*glm::perlin(glm::vec2(i/10.0,j/10.0)), j), Vector3f(0, 0, 0), Vector3f(0, 1, 0)));
			Vertices.push_back(VertexTex3D(Vector3f(i, 5*glm::perlin(glm::vec2(i/10.0, (j+1)/10.0)), j+1), Vector3f(0, 1, 0), Vector3f(0, 1, 0)));
			Vertices.push_back(VertexTex3D(Vector3f(i+1, 5*glm::perlin(glm::vec2((i+1)/10.0, j/10.0)), j), Vector3f(1, 0, 0), Vector3f(0, 1, 0)));
			Vertices.push_back(VertexTex3D(Vector3f(i+1, 5*glm::perlin(glm::vec2((i+1)/10.0, (j+1)/10.0)), j+1), Vector3f(1, 1, 0), Vector3f(0, 1, 0)));

			Indices.push_back(curr_ind);
			Indices.push_back(curr_ind+1);
			Indices.push_back(curr_ind+2);
			Indices.push_back(curr_ind+3);
			Indices.push_back(curr_ind+2);
			Indices.push_back(curr_ind+1);

			curr_ind += 4;
		}
	}

	//post-processing
	for (int i = 0;i < Vertices.size();i++)
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
	}

	mMesh.init(Vertices, Indices);
}

void Terrain::render()
{
	mMesh.render();
}
