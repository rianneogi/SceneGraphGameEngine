#pragma once

#include "GameObject.h"

class MeshRenderer : public GameComponent
{
public:
	Model mModel;

	MeshRenderer();
	~MeshRenderer();

	void render();
};

