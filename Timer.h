#pragma once
#ifndef TIMER_H
#define TIMER_H

#include <SDL3/SDL.h>

class Timer {
public:
	Timer();
	~Timer();

	void Start();
	void UpdateTicks();
	float GetDeltaTime() const;
	float GetTotalTime() const;

	bool IsFixedUpdateDue(); // Used for fixed time step updates to avoid frame rate dependency
	void ResetTimer();

private:
	uint64_t lastTicks;
	uint64_t currentTicks;
	float deltaTime; // Time between frames
	float totalTime; // Total time since the start of the application

	float fixedTimeStep = 0.0166f; // Fixed time step for updates (60 FPS)
	float fixedTimer = 0.0f;
};

#endif // !TIMER_H

