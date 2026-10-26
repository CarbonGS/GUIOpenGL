#include "Texture.h"
#include "tiny_obj_loader.h"

Texture::Texture() : textureID(0)
{
}

Texture::~Texture()
{
	glDeleteTextures(1, &textureID);
}

bool Texture::LoadTexture(const std::string& filePath)
{
	SDL_Surface* surface = IMG_Load(filePath.c_str());
	if (!surface) {
		SDL_Log("Failed to load texture");
		return false;
	}
	SDL_Surface* converted = SDL_ConvertSurface(surface, SDL_PIXELFORMAT_RGBA32);
	if (!converted) {
		SDL_Log("Failed to convert surface");
		SDL_DestroySurface(surface);
		return false;
	}
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, converted->w, converted->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, converted->pixels);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);
	SDL_DestroySurface(converted);
	SDL_DestroySurface(surface);
	return true;
}
