#pragma once

#include "LoadResources.h"

class Game
{
public:
	Game();
	~Game();

	Camera mCamera;
	SkyBox* mSkybox;

	std::vector<ShaderProgram*> mShaders;
	Terrain* mTerrain;
	std::vector<Texture*> mTextures;
	CubeMapTexture* mSkyTexture;
	std::vector<Mesh*> mMeshes;

	bool init();
	void cleanup();

	void render(SDL_Window* window);
	void update(int deltaTime);
	void handleEvent(SDL_Event e, int deltaTime);
};

