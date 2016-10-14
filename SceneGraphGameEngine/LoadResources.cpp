#include "LoadResources.h"

const int TEXTURE_WIDTH = 16;
const int TEXTURE_HEIGHT = 16;
const int BYTES_PER_TEXEL = 4;

std::vector<Texture*> gTileTextures;
TextureArray* gTerrain3DTexture;

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

	gTerrain3DTexture = new TextureArray();
	res = res && gTerrain3DTexture->loadFromFile(gTileTextures, TEXTURE_WIDTH, TEXTURE_HEIGHT, BYTES_PER_TEXEL);
	
	return res;
}