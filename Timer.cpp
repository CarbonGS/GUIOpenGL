#include "Timer.h"

Timer::Timer() : lastTicks(0), currentTicks(0), deltaTime(0.0f), totalTime(0.0f), fixedTimer(0.0f) {}

Timer::~Timer() {}

void Timer::Start()
{
	lastTicks = SDL_GetTicks();
	currentTicks = SDL_GetTicks();
}

void Timer::UpdateTicks()
{
	lastTicks = currentTicks;
	currentTicks = SDL_GetTicks();
	

	deltaTime = (currentTicks - lastTicks) / 1000.0f; // Convert milliseconds to seconds
	totalTime += deltaTime;
	fixedTimer += deltaTime;
}

float Timer::GetDeltaTime() const
{
	return deltaTime;
}

float Timer::GetTotalTime() const
{
	return totalTime;
}

bool Timer::IsFixedUpdateDue()
{
	if (fixedTimer >= fixedTimeStep)
	{
		fixedTimer -= fixedTimeStep; // Reset the fixed timer
		return true; // Fixed update is due
	}
	return false; // Not due yet
}

void Timer::ResetTimer()
{
	lastTicks = SDL_GetTicks();
	currentTicks = lastTicks;
	deltaTime = 0.0f;
	totalTime = 0.0f;
	fixedTimer = 0.0f; // Reset fixed timer
}
