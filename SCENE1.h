#pragma once

#include "SDL3\SDL.h"
#include <SDL3\SDL_video.h>
#include "SDL3\SDL_keyboard.h"

#include <glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Scene.h"
#include "Shader.h"
#include "Mesh.h"
#include "Math.h"
#include "Texture.h"
#include "System.h"

class UI;

class Scene1 : public Scene {
private:
	Shader* shader;
	Mesh* car;
	System* physics;
	glm::vec3 force;
	Mesh* road;
	UI* ui;
	UI* shiftUI;

	int speed;

	bool movingForward = false;
	bool movingBackward = false;

	glm::vec3 lightPosition;
	
	glm::mat4 modelMatrix;
	glm::mat4 roadMatrix;
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
	glm::mat4 ortho;

public:
	explicit Scene1();
	virtual ~Scene1();

	virtual int SceneCreate(SDL_Window* window) override;
	virtual void SceneDestroy() override;
	virtual void Update(const float deltaTime) override;
	virtual void Render() const override;
	virtual void HandleEvents(const SDL_Event& sdlEvent) override;
};