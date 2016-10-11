#pragma once

#include "Camera.h"

class Timer
{
public:
	Timer();
	~Timer();

	Uint32 getElaspedTime();
	void restart();

	Uint32 mStartTime;
};

