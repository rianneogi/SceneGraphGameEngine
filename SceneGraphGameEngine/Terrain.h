#pragma once

#include "Timer.h"

class Terrain
{
public:
	MeshDataTexture3D mMesh;
	Texture mTexture;

	Terrain();
	~Terrain();

	void generate();
	void render();
};

