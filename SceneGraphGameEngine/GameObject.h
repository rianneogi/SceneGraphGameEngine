#pragma once

#include "GameComponent.h"

class GameObject
{
public:
	GameObject();
	~GameObject();

	std::vector<GameObject*> mChildren;
	std::vector<GameComponent*> mComponents;

	void input();
	void render();
	void update();
};

