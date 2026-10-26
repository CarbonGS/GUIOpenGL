#pragma once
#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <GL/glew.h>
#include <SDL3/SDL.h>
#include "SDL3\SDL_events.h"
#include "Window.h"
#include "Timer.h"
#include "Scene1.h"

class SceneManager {
public:
	SceneManager() : currentScene(nullptr), selectScene(1), window(nullptr), timer(nullptr) {
		currentScene = nullptr;
	}
	~SceneManager() { cleanup(); }

	int selectScene;
	Scene* currentScene;
	Window* window;
	Timer* timer;

	void switchScene(int selectedScene);

	void StartApp();
	bool quit = false;

	void Run();
	void VersionInfo();
	void Loop();
	void cleanup();
	void HandleEvents();
};

#endif // !SCENEMANAGER_H
