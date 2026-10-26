#include "SCENE1.h"
#include "UI.h"

Scene1::Scene1()
	: shader(nullptr), car(nullptr),
	modelMatrix(glm::mat4(1.0f)), viewMatrix(glm::mat4(1.0f)), 
	projectionMatrix(glm::mat4(1.0f)), lightPosition(glm::vec3(0.0f)),
	physics(nullptr) , force(glm::vec3(0.0f)) { }

Scene1::~Scene1() 
{
	SceneDestroy();
}

void Scene1::SceneDestroy() 
{
	if (shader) {
		delete shader;
		shader = nullptr;
	}
	if (car) {
		delete car;
		car = nullptr;
	}
	if(physics) {
		delete physics;
		physics = nullptr;
	}
	if(road) {
		delete road;
		road = nullptr;
	}
	if (ui) {
		delete ui;
		ui = nullptr;
	}
}

int Scene1::SceneCreate(SDL_Window* window)
{

	physics = new System();
	physics->pos = glm::vec3(0.0f, 0.0f, 0.0f);
	physics->mass = 1290.0f;
	physics->frontMeters = 2.1f;
	physics->dragCoef = 0.30f;
	physics->efficiency = 0.86f;
	physics->sideWall = 225.0f * 0.4f / 10.0f; // mm to cm
	physics->rimRadius = 18.0f * 2.54f / 2.0f; // " to cm
	physics->wheelRadius = (physics->sideWall + physics->rimRadius) / 100.0f; // Meters
	physics->rollingCoef = 30.0f;

	physics->torqueCurve = {
		{ 1000.0f, 180 },
		{ 2000.0f, 220 },
		{ 3000.0f, 250 },
		{ 4000.0f, 270 },
		{ 4000.0f, 280 },
		{ 5000.0f, 270 },
		{ 6000.0f, 250 },
		{ 7000.0f, 220 }
	};

	car = new Mesh();
	car->CreateMesh("Meshes/brz.obj");
	car->MeshToGPU();

	road = new Mesh();
	road->CreateMesh("Meshes/road.obj");
	road->MeshToGPU();

	shader = new Shader("Shaders/Vertex.glsl", "Shaders/Fragment.glsl");
	if (shader->CreateShaderProgram() != 0) {
		std::cerr << "Failed to create shader program." << std::endl;
		return -1; // Error creating shader program
	}

	ui = new UI();
	markup_t markup;
	markup.family = const_cast<char*>("orb");
	markup.size = 24.0f;
	markup.bold = false;
	markup.italic = false;
	markup.foreground_color = vec4{ 1.0f, 1.0f, 1.0f, 1.0f }; // White color
	ui->loadFont("Fonts/orb.ttf", &markup);

	shiftUI = new UI();
	markup_t shiftMarkup;
	shiftMarkup.family = const_cast<char*>("orb");
	shiftMarkup.size = 42.0f;
	shiftMarkup.bold = false;
	shiftMarkup.italic = false;
	shiftMarkup.foreground_color = vec4{ 1.0f, 0.0f, 0.0f, 1.0f };
	shiftUI->loadFont("Fonts/orb.ttf", &shiftMarkup);

	// Matrix setup
	constexpr float fov = glm::radians(45.0f);   // Convert degrees to radians
	glm::mat4 projMatrix = glm::perspective(fov, 1280.0f/720.0f, 0.1f, 300.0f);
	glm::mat4 vwMatrix = glm::lookAt(
		glm::vec3(0.0f, 0.0f, 8.0f), // Camera position
		glm::vec3(0.0f, 0.0f, 0.0f), // Target
		glm::vec3(0.0f, 1.0f, 0.0f)  // Up
	);

	lightPosition = glm::vec3(10.0f, 10.0f, 9.0f); // Set light position

	// Set the matrices
	modelMatrix = physics->getModelMatrix();
	projectionMatrix = projMatrix;
	viewMatrix = vwMatrix;
	roadMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 20.0f));
	ortho = glm::ortho(0.0f, 1280.0f, 0.0f, 720.0f, -1.0f, 1.0f); 

	return 0;
}

void Scene1::Update(const float deltaTime)
{
	force = glm::vec3(0.0f);
	speed = glm::length(physics->vel) * 3.6f;

	if (speed > 50 && physics->gear == 0) physics->gear = 1;
	if (speed > 90 && physics->gear == 1) physics->gear = 2;
	if (speed > 133 && physics->gear == 2) physics->gear = 3;
	if (speed > 160 && physics->gear == 3) physics->gear = 4;
	if (speed > 188 && physics->gear == 4) physics->gear = 5;

	force = physics->CalculateForce(deltaTime, movingForward);
	physics->ApplyForce(force);
	physics->UpdateVelocity(deltaTime);
	physics->UpdatePosition(deltaTime);
	modelMatrix = physics->getModelMatrix();

	float rpm = physics->CalculateRPM();

	glm::vec3 carPos = physics->pos;
	glm::vec3 camOffset(0.0f, 2.3f, -7.0f); // Camera offset from the car
	float shakeIntensity = glm::clamp(glm::length(physics->vel) * 0.0002f, 0.0f, 0.2f); // scale as needed
	glm::vec3 shakeOffset = glm::vec3(
		(rand() % 100 / 100.0f - 0.5f) * shakeIntensity,
		(rand() % 100 / 100.0f - 0.5f) * shakeIntensity,
		0.0f
	);

	glm::vec3 shakenCamOffset = camOffset + shakeOffset;

	viewMatrix = glm::lookAt(
		carPos + shakenCamOffset,
		carPos + glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f)
	);

}

void Scene1::Render() const
{
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE); // Disable face culling
	glEnable(GL_DEPTH_TEST); // Enable depth testing

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glUseProgram(shader->GetShaderID());

	GLint projLoc = glGetUniformLocation(shader->GetShaderID(), "projectionMatrix");
	GLint viewLoc = glGetUniformLocation(shader->GetShaderID(), "viewMatrix");
	GLint modelLoc = glGetUniformLocation(shader->GetShaderID(), "modelMatrix");

	glUniformMatrix4fv(projLoc, 1, GL_FALSE, &projectionMatrix[0][0]);
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &viewMatrix[0][0]);
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &modelMatrix[0][0]);

	GLint lightPosLoc = glGetUniformLocation(shader->GetShaderID(), "lightPos");
	GLint viewPosLoc = glGetUniformLocation(shader->GetShaderID(), "viewPos");
	GLint lightColorLoc = glGetUniformLocation(shader->GetShaderID(), "lightColor");
	GLint ambientColorLoc = glGetUniformLocation(shader->GetShaderID(), "ambientColor");

	glm::vec3 lightPos = lightPosition;
	glm::mat4 view = viewMatrix;
	glm::vec3 viewPos = glm::vec3(glm::inverse(view)[3]);
	glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 ambient = glm::vec3(0.4f, 0.4f, 0.4f);

	// Upload to GPU
	glUniform3fv(lightPosLoc, 1, &lightPos[0]);
	glUniform3fv(viewPosLoc, 1, &viewPos[0]);
	glUniform3fv(lightColorLoc, 1, &lightColor[0]);
	glUniform3fv(ambientColorLoc, 1, &ambient[0]);


	for (size_t i = 0; i < car->loadedTextures.size(); ++i) {
		if (car->loadedTextures[i]) {
			glActiveTexture(GL_TEXTURE0 + static_cast<GLenum>(i));
			glBindTexture(GL_TEXTURE_2D, car->loadedTextures[i]->GetTextureID());

			std::string uniformName = "textures[" + std::to_string(i) + "]";
			GLint loc = glGetUniformLocation(shader->GetShaderID(), uniformName.c_str());
			glUniform1i(loc, static_cast<GLint>(i));
		}
	}
	car->RenderMesh();
	
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &roadMatrix[0][0]);

	for (size_t i = 0; i < road->loadedTextures.size(); ++i) {
		if (road->loadedTextures[i]) {
			glActiveTexture(GL_TEXTURE0 + static_cast<GLenum>(i));
			glBindTexture(GL_TEXTURE_2D, road->loadedTextures[i]->GetTextureID());
			std::string uniformName = "textures[" + std::to_string(i) + "]";
			GLint loc = glGetUniformLocation(shader->GetShaderID(), uniformName.c_str());
			glUniform1i(loc, static_cast<GLint>(i));
		}
	}

	road->RenderMesh();

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glm::vec4 color = glm::vec4(1, 1, 1, 1);

	ui->clearUI();
	ui->loadText(std::to_string(static_cast<int>(glm::length(physics->vel) * 3.6f)) + " km/h", 1100.0f, 50.0f, 3.0f);
	ui->TextToGPU();
	ui->render(color, ortho);

	color = glm::vec4(1, 0, 0, 1); // Red color for shift UI
	float rpm = physics->CalculateRPM();

	if (rpm >= 6800) {
		shiftUI->clearUI();
		shiftUI->loadText("SHIFT UP +", 550.0f, 700.0f, 1.0f);
		shiftUI->TextToGPU();
		shiftUI->render(color, ortho);
	}
	
	glEnable(GL_DEPTH_TEST);

	glUseProgram(0); // Unbind the shader program
}

void Scene1::HandleEvents(const SDL_Event& sdlEvent)
{
	if (sdlEvent.type == SDL_EVENT_KEY_DOWN && !sdlEvent.key.repeat) {
		switch (sdlEvent.key.scancode) {
		case SDL_SCANCODE_W:
			movingForward = true;
			break;
		case SDL_SCANCODE_S:
			movingBackward = true;
			break;
		}
	}
	if (sdlEvent.type == SDL_EVENT_KEY_UP && !sdlEvent.key.repeat) {
		switch (sdlEvent.key.scancode) {
		case SDL_SCANCODE_W:
			movingForward = false;
			break;
		case SDL_SCANCODE_S:
			movingBackward = false;
			break;
		}
	}
}

