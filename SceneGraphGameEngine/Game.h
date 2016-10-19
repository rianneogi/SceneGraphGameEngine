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
	CubeMapTexture* mDayTexture;
	CubeMapTexture* mNightTexture;
	std::vector<Mesh*> mMeshes;
	FrameBuffer mShadowFBO;

	Water* mWater;

	std::vector<glm::mat4> mEntities;

	bool init();
	void cleanup();

	void render(SDL_Window* window);
	void update(int deltaTime);
	void handleEvent(SDL_Event e, int deltaTime);

	void renderSkybox(const glm::mat4& view, const glm::mat4& projection);
	void renderScene(const glm::mat4& view, const glm::mat4& projection);
};

