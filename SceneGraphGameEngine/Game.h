#pragma once

#include "Terrain.h"

class Game
{
public:
	Game();
	~Game();

	Camera mCamera;

	std::vector<ShaderProgram> mShaders;
	Terrain mTerrain;
	Texture t;
	Mesh m;

	bool init();

	void render(SDL_Window* window);
	void update(int deltaTime);
	void handleEvent(SDL_Event e, int deltaTime);
};

