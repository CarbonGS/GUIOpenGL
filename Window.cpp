#pragma once
#include "Window.h"
#include <GLFW\glfw3.h>

Window::Window() : window{ nullptr }, openGLContext{ nullptr }, windowWidth{ 0 }, windowHeight{ 0 } {}

Window::~Window()
{
	cleanup();
}

int Window::initialize()
{

	windowWidth = 1280; // Default width
	windowHeight = 720; // Default height

	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		SDL_Log("Could not initialize SDL: %s", SDL_GetError());
		std::cout << "SDL Initialization failed: " << SDL_GetError() << std::endl;
		return 1;
	}

	// SDL Version
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

	// SDL Core (non deprecated) profile
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	// SDL Buffering
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	// Create the SDL window with OpenGL context
	window = SDL_CreateWindow("OpenGL Engine", windowWidth, windowHeight, SDL_WINDOW_OPENGL);
	id = SDL_GetWindowID(window);

	if (!window) {
		SDL_Log("Could not create window: %s", SDL_GetError());
		std::cout << "Window creation failed: " << SDL_GetError() << std::endl;
		return 1;
	}

	// Set the OpenGL context for the window
	openGLContext = SDL_GL_CreateContext(window);

	if (!openGLContext) {
		SDL_Log("Could not create OpenGL context: %s", SDL_GetError());
		std::cout << "OpenGL context creation failed: " << SDL_GetError() << std::endl;
		return 1;
	}

	SDL_GL_MakeCurrent(window, openGLContext); // Make the OpenGL context current

	// Initialize GLEW
	glewExperimental = GL_TRUE; // Enable experimental features
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		std::cout << "GLEW Initialization failed: " << glewGetErrorString(err) << std::endl;
		return 1;
	}

	if (TTF_Init() == -1) {
		std::cout << "SDL_ttf Initialization failed: " << SDL_GetError() << std::endl;
	}

	glViewport(0, 0, windowWidth, windowHeight); // Set the viewport to the window size
	std::cout << "\nOpenGL Engine initialized successfully!\n" << std::endl;
	return 0; // Success
}

void Window::cleanup()
{
	if (openGLContext) {
		SDL_GL_DestroyContext(openGLContext);
		openGLContext = nullptr;
	}
	if (window) {
		SDL_DestroyWindow(window);
		window = nullptr;
	}
	if (TTF_WasInit()) {
		TTF_Quit();
	}
	SDL_Quit();
}