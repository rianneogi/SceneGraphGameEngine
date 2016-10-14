#include "LoadResources.h"

const int TEXTURE_WIDTH = 16;
const int TEXTURE_HEIGHT = 16;
const int BYTES_PER_TEXEL = 4;

std::vector<Texture*> gTileTextures;
GLuint gTerrain3DTexture;

bool generateTextureArray()
{
	GLenum err = GL_NO_ERROR;
	//typedef char BYTE;
	//allocate memory for the texels
	//typedef unsigned char BYTE;
	GLubyte* texels = (GLubyte*)malloc(TEXTURE_WIDTH * TEXTURE_HEIGHT * gTileTextures.size() * BYTES_PER_TEXEL);
	for (size_t i = 0;i < gTileTextures.size();i++)
	{
		//sf::Image img = gTextures[i].mTex.copyToImage();
		gTileTextures[i]->bind();
		glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, &texels[BYTES_PER_TEXEL*TEXTURE_WIDTH*TEXTURE_HEIGHT*i]);
		//glReadPixels(0, 0, TEXTURE_WIDTH, TEXTURE_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, &texels[BYTES_PER_TEXEL*TEXTURE_WIDTH*TEXTURE_HEIGHT*i]);
		//for (int j = 0;j < TEXTURE_HEIGHT;j++)
		//{
		//	for (int k = 0;k < TEXTURE_WIDTH;k++)
		//	{
		//		//sf::Color col = img.getPixel(j, k);
		//		//printf("pixel %d %d %d %d\n", col.r, col.g, col.b, col.a);
		//		texels[BYTES_PER_TEXEL*(TEXTURE_WIDTH*TEXTURE_HEIGHT*i + TEXTURE_WIDTH*j + k)] = 255;
		//		texels[BYTES_PER_TEXEL*(TEXTURE_WIDTH*TEXTURE_HEIGHT*i + TEXTURE_WIDTH*j + k) + 1] = 0;
		//		texels[BYTES_PER_TEXEL*(TEXTURE_WIDTH*TEXTURE_HEIGHT*i + TEXTURE_WIDTH*j + k) + 2] = 255;
		//		texels[BYTES_PER_TEXEL*(TEXTURE_WIDTH*TEXTURE_HEIGHT*i + TEXTURE_WIDTH*j + k) + 3] = 127;
		//	}
		//}
	}

	glGenTextures(1, &gTerrain3DTexture);
	glBindTexture(GL_TEXTURE_2D_ARRAY, gTerrain3DTexture);

	while ((err = glGetError()) != GL_NO_ERROR)
	{
		printf("texture generation 1 ERROR: %d\n", err);
	}
	//glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA, TEXTURE_WIDTH, TEXTURE_HEIGHT, TEXTURE_DEPTH);
	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_SRGB_ALPHA, TEXTURE_WIDTH, TEXTURE_HEIGHT, gTileTextures.size(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	//glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA, TEXTURE_WIDTH, TEXTURE_HEIGHT, TEXTURE_DEPTH, 0, GL_RGBA, GL_UNSIGNED_BYTE, texels);
	for (int i = 0;i < gTileTextures.size();i++)
	{
		glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, TEXTURE_WIDTH, TEXTURE_HEIGHT, 1, GL_RGBA, GL_UNSIGNED_BYTE, &texels[BYTES_PER_TEXEL*TEXTURE_WIDTH*TEXTURE_HEIGHT*i]);
	}

	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glGenerateMipmap(GL_TEXTURE_2D_ARRAY);

	free(texels);

	while ((err = glGetError()) != GL_NO_ERROR)
	{
		printf("texture generation 2 ERROR: %d\n", err);
	}

	return true;
}

bool loadTileTexture(std::string path)
{
	gTileTextures.push_back(new Texture());
	if (!gTileTextures[gTileTextures.size()-1]->loadFromFile(path))
	{
		printf("Unable to load texture: %s\n", path);
		return false;
	}
	return true;
}

bool loadTextures()
{
	bool res = loadTileTexture("Resources\\Textures\\cobblestone.png");
	res = res && loadTileTexture("Resources\\Textures\\dirt.png");
	res = res && loadTileTexture("Resources\\Textures\\brick.png");
	res = res && loadTileTexture("Resources\\Textures\\sand.png");
	
	return res && generateTextureArray();
}