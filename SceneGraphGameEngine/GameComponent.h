#pragma once

#include "Timer.h"

class GameComponent
{
public:
	GameComponent();
	~GameComponent();

	void input();
	void render();
	void update();
};

