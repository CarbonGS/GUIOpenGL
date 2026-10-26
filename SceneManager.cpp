#include "SceneManager.h"
#include <intrin.h>

void SceneManager::switchScene(int selectedScene)
{
	if (currentScene != nullptr) {
		currentScene->SceneDestroy();
		delete currentScene;
		currentScene = nullptr;
	}

	switch (selectedScene) {
	case 1:
		currentScene = new Scene1();
		currentScene->SceneCreate(window->getWindow());
		break;
	}
}

void SceneManager::StartApp()
{
	SceneManager* manager = new SceneManager();
	manager->Run();
	delete manager;
}

void SceneManager::Run()
{
	std::cout << "Starting OpenGL Engine..." << std::endl;

	window = new Window();
	if(window->initialize() != 0) {
		std::cout << "Initialization failed. Exiting..." << std::endl;
		return;
	}

	VersionInfo();

	switchScene(selectScene); // Initial level load

	timer = new Timer();
	timer->Start();

	Loop();
}

void SceneManager::VersionInfo()
{
	std::cout << "System Information:\n" << std::endl;
	std::cout << "CPU Architecture: " << (sizeof(void*) == 8 ? "64-bit" : "32-bit") << std::endl;
	std::cout << "CPU Cores: " << SDL_GetNumLogicalCPUCores() << std::endl;
	int cpuInfo[4] = { 0 };
	char brand[49] = { 0 };
	__cpuid(cpuInfo, 0x80000002);
	memcpy(brand, cpuInfo, sizeof(cpuInfo));
	__cpuid(cpuInfo, 0x80000003);
	memcpy(brand + 16, cpuInfo, sizeof(cpuInfo));
	__cpuid(cpuInfo, 0x80000004);
	memcpy(brand + 32, cpuInfo, sizeof(cpuInfo));
	std::cout << "CPU Model: " << brand << std::endl;

	std::cout << "\nOperating System: " << SDL_GetPlatform() << std::endl;

	std::cout << "\nGPU Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "GPU Renderer: " << glGetString(GL_RENDERER) << std::endl;

	std::cout << "\nOpenGL Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "OpenGL Shading Language Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	std::cout << "\nGLEW Version: " << glewGetString(GLEW_VERSION) << std::endl;
	std::cout << "GLEW Compatibility: " << (GLEW_VERSION_4_6 ? "OpenGL 4.6" : "Not OpenGL 4.6 compatible") << std::endl;
}

void SceneManager::Loop()
{
	while (!quit) {
		timer->UpdateTicks();

		HandleEvents();

		currentScene->Update(timer->GetDeltaTime());
		currentScene->Render();

		// Update the window
		SDL_GL_SwapWindow(window->getWindow());
	}
}

void SceneManager::cleanup()
{
	if (currentScene) {
		currentScene->SceneDestroy();
		delete currentScene;
		currentScene = nullptr;
	}
	if (timer) {
		delete timer;
		timer = nullptr;
	}
}

void SceneManager::HandleEvents()
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_EVENT_QUIT:
			quit = true;
			break;
		case SDL_EVENT_KEY_DOWN:
			if (event.key.scancode == SDL_SCANCODE_ESCAPE) {
				quit = true;
			}
			currentScene->HandleEvents(event);
			break;
		default:
			currentScene->HandleEvents(event);
			break;
		}
	}
}
