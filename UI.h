#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_ttf.h>
#include "SceneManager.h"
#include <iostream>
#include <string>
#include "Window.h"
#include "Shader.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include <freetype-gl-1.0\freetype-gl.h>
#include <freetype-gl-1.0\font-manager.h>
#include <freetype-gl-1.0\markup.h>
#include <freetype-gl-1.0\text-buffer.h>

class UI {

private:
	texture_atlas_t* atlas;
	font_manager_t* manager;
	text_buffer_t* buffer;
	texture_font_t* font;
	markup_t* markup;
	vec2 pen;

	Shader* uiShader;
	GLuint vao, vbo;

public:
	UI();
	~UI();

	void loadFont(const std::string& fontPath, markup_t* markup);
	void loadText(const std::string& text, float x, float y, float scale);
	void render(const glm::vec4 color, const glm::mat4& projectionMatrix);
	void clearUI();
	void TextToGPU();
	void updateTextBuffer();
	void updateAtlasTexture();
};
