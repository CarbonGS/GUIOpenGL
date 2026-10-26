// Project: OpenGL Engine

#include "Timer.h"
#include <iostream>
#include <filesystem>
#include "SceneManager.h"

int main(int argc, char* argv[])
{	
	
	SceneManager sceneManager;
	sceneManager.selectScene = 1; // Select Scene1
	sceneManager.StartApp();

	return 0;
}