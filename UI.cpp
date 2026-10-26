#include "UI.h"

UI::UI()
{
	uiShader = new Shader("Shaders/TextVert.glsl", "Shaders/TextFrag.glsl");
	if (uiShader->CreateShaderProgram() != 0) {
		std::cerr << "Failed to create TEXT shader program." << std::endl;
		return; // Error creating text shader program
	}
	markup = new markup_t();
}

UI::~UI()
{
	if (markup) {
		delete markup;
		markup = nullptr;
	}
	if (uiShader) {
		delete uiShader;
		uiShader = nullptr;
	}
	if(vbo) {
		glDeleteBuffers(1, &vbo);
		vbo = 0;
	}
	if (vao) {
		glDeleteVertexArrays(1, &vao);
		vao = 0;
	}
	if (buffer) {
		text_buffer_delete(buffer);
		buffer = nullptr;
	}
	if (font) {
		texture_font_delete(font);
		font = nullptr;
	}
	if (manager) {
		font_manager_delete(manager);
		manager = nullptr;
	}
	if (atlas) {
		texture_atlas_delete(atlas);
		atlas = nullptr;
	}
}

void UI::loadFont(const std::string& fontPath, markup_t* markup_)
{
	atlas = texture_atlas_new(512, 512, 1);
	if (!atlas) {
		std::cerr << "Failed to create texture atlas." << std::endl;
		return; // Error creating texture atlas
	}
	
	glGenTextures(1, &atlas->id);
	glBindTexture(GL_TEXTURE_2D, atlas->id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, atlas->width, atlas->height, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	font = texture_font_new_from_file(atlas, markup_->size, fontPath.c_str());
	if (!font) {
		std::cerr << "Failed to load font from: " << fontPath << std::endl;
		return; // Error loading font
	}

	manager = font_manager_new(512, 512, 1);
	if (!manager) {
		std::cerr << "Failed to create font manager." << std::endl;
		return; // Error creating font manager
	}

	markup->family = markup_->family;
	markup->size = markup_->size;
	markup->bold = markup_->bold;
	markup->italic = markup_->italic;
	markup->foreground_color = markup_->foreground_color;
	markup->font = font;

	buffer = text_buffer_new();
	if (!buffer) {
		std::cerr << "Failed to create text buffer." << std::endl;
		return; // Error creating text buffer
	}

}

void UI::loadText(const std::string& text, float x, float y, float multiplyScale)
{
	pen.x = x;
	pen.y = y;

	if (multiplyScale <= 0.0f) {
		markup->size *= 1;
	}
	else {
		markup->size *= multiplyScale;
	}


	for (size_t i = 0; i < text.length(); ++i) {
		texture_font_get_glyph(font, &text[i]);
	}

	updateAtlasTexture();

	text_buffer_add_text(buffer, &pen, markup, text.c_str(), text.length());

	if (buffer->buffer->vertices->size > 0) {
		float* vertexData = (float*)buffer->buffer->vertices->items;
	}
}

void UI::render(const glm::vec4 color, const glm::mat4& projectionMatrix)
{
	uiShader->Use();

	uiShader->setMat4("orthoMatrix", projectionMatrix);
	uiShader->setVec4("textColor", color);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, atlas->id);

	GLint textLoc = glGetUniformLocation(uiShader->GetShaderID(), "textTexture");
	glUniform1i(textLoc, 0);

	GLint width, height;
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);

	glBindVertexArray(vao);
	vertex_buffer_render(buffer->buffer, GL_TRIANGLES);
	glBindVertexArray(0);

}

void UI::clearUI()
{
	if(buffer) {
		text_buffer_clear(buffer);
	}
}

void UI::TextToGPU()
{
	if (vao) {
		glDeleteVertexArrays(1, &vao);
	}
	if (vbo) {
		glDeleteBuffers(1, &vbo);
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	size_t size = buffer->buffer->vertices->size * buffer->buffer->vertices->item_size;
	void* vertexData = buffer->buffer->vertices->items;

	glBufferData(GL_ARRAY_BUFFER, size, vertexData, GL_DYNAMIC_DRAW);

	size_t stride = buffer->buffer->vertices->item_size;

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

void UI::updateTextBuffer()
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	size_t dataSize = buffer->buffer->vertices->size * buffer->buffer->vertices->item_size;
	void* vertexData = buffer->buffer->vertices->items;
	glBufferSubData(GL_ARRAY_BUFFER, 0, dataSize, vertexData);
}

void UI::updateAtlasTexture()
{
	if (atlas && atlas->id) {
		glBindTexture(GL_TEXTURE_2D, atlas->id);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, atlas->width, atlas->height, GL_RED, GL_UNSIGNED_BYTE, atlas->data);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}