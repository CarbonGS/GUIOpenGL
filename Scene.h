#pragma once
#ifndef SCENE_H
#define SCENE_H
#include <SDL3/SDL.h>

union SDL_Event;

class Scene {
public:
	Scene() {}
	virtual ~Scene() {}

	virtual int SceneCreate(SDL_Window* window) = 0;
	virtual void SceneDestroy() = 0;
	virtual void Update(const float deltaTime) = 0;
	virtual void Render() const = 0;
	virtual void HandleEvents(const SDL_Event& sdlEvent) = 0;
};

#endif // !SCENE_H
