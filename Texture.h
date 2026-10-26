#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL3/SDL_image.h>
#include <GL\glew.h>
#include <string>
#include <SDL3\SDL.h>

class Texture {
private:
	GLuint textureID;

public:
	Texture();
	~Texture();

	bool LoadTexture(const std::string& filePath);
	GLuint GetTextureID() const { return textureID; }
};

#endif // !TEXTURE_H
