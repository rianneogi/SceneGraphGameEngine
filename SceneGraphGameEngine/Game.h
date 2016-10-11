#pragma once

#include "GameObject.h"

class Game
{
public:
	Game();
	~Game();

	Camera mCamera;

	Mesh m;
	std::vector<ShaderProgram> mShaders;
	Texture t;

	bool init();

	void render(SDL_Window* window);
	void update(int deltaTime);
	void handleEvent(SDL_Event e, int deltaTime);
};

