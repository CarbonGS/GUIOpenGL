#pragma once
#ifndef WINDOW_H
#define WINDOW_H

#include <SDL3/SDL.h>
#include <GL/glew.h>
#include <iostream>
#include "SDL3\SDL_ttf.h"

class Window {

private:
	int windowWidth, windowHeight;
	int id; // SDL window ID

	SDL_Window* window;
	SDL_GLContext openGLContext;

public:

	Window();
	~Window();

	int initialize();
	void cleanup();

	int getWindowWidth() const { return windowWidth; }
	int getWindowHeight() const { return windowHeight; }
	SDL_Window* getWindow() const { return window; }
};

#endif // !WINDOW_H

