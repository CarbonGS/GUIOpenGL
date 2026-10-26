#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <iostream>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
private:
	GLuint shaderID; // Shader program ID
	GLuint vertexShaderID;
	GLuint fragmentShaderID;
	GLuint tessControlShaderID;
	GLuint tessEvalShaderID;

	const char* vertexShaderPath;
	const char* fragmentShaderPath;
	const char* tessControlShaderPath;
	const char* tessEvalShaderPath;
	std::unordered_map<std::string, GLuint > uniMap;

	std::string LoadShaderFile(const std::string& filePath);

public:
	Shader(
		const char* vertexShaderPath = nullptr,
		const char* fragmentShaderPath = nullptr,
		const char* tessControlShaderPath = nullptr,
		const char* tessEvalShaderPath = nullptr);
	~Shader();

	int CreateShaderProgram();
	GLuint GetShaderID() const { return shaderID; }
	GLuint GetUniformID(std::string name);

	void Use() const;
	void setVec4(const std::string& name, const glm::vec4& value) const;
	void setMat4(const std::string& name, const glm::mat4& value) const;
	

};

#endif // !SHADER_H