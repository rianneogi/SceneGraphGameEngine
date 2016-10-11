#include "Timer.h"

Timer::Timer()
{
	restart();
}

Timer::~Timer()
{
}

Uint32 Timer::getElaspedTime()
{
	return (SDL_GetTicks() - mStartTime);
}

void Timer::restart()
{
	mStartTime = SDL_GetTicks();
}