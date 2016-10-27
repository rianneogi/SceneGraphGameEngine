#pragma once

#include "LoadResources.h"
#include <map>

class Game
{
public:
	Game();
	~Game();

	Camera mCamera;
	SkyBox* mSkybox;

	glm::vec3 mSunDirection;

	std::vector<ShaderProgram*> mShaders;
	std::vector<Texture*> mTextures;
	CubeMapTexture* mDayTexture;
	CubeMapTexture* mNightTexture;
	std::vector<Mesh*> mMeshes;
	FrameBuffer mShadowFBO;

	Water* mWater;

	std::vector<glm::mat4> mEntities;
	std::map<std::string, Terrain*> mTerrainChunks;

	bool init();

	void render(SDL_Window* window);
	void update(int deltaTime);
	void handleEvent(SDL_Event e, int deltaTime);

	void renderSkybox(const glm::mat4& view, const glm::mat4& projection);
	void renderScene(const glm::mat4& view, const glm::mat4& projection);
	void renderWithShadows(const glm::mat4& view, const glm::mat4& projection);

	void updateChunks();
	void renderTerrain(const glm::mat4& view, const glm::mat4& projection);
	std::string getChunkString(int i, int j);
};

Vector2i getAligned(double x, double y, double period);
Vector3i getAligned(double x, double y, double z, double period);
