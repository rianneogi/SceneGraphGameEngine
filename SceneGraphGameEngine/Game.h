#pragma once

#include "Terrain.h"

class Game
{
public:
	Game();
	~Game();

	bool loadTileTexture(std::string path);
	bool loadTextures();

	Camera mCamera;
	SkyBox* mSkybox;

	Renderer mRenderer;

	glm::vec3 mSunDirection;

	std::vector<Texture2D*> mTextures;
	std::vector<Material*> mMaterials;
	std::vector<MeshDataTex*> mMeshes;
	std::vector<Model*> mModels;

	std::vector<Texture2D*> mTileTextures;
	TextureArray* mTerrainTexture;
	Material* mTerrainMaterial;

	CubeMapTexture* mDayTexture;
	CubeMapTexture* mNightTexture;
	
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
