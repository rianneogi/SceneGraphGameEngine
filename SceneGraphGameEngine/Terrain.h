#pragma once

#include "Timer.h"

class Terrain
{
public:
	MeshData mMesh;
	Texture mTexture;

	Terrain();
	~Terrain();

	void generate();
	void render();
};

