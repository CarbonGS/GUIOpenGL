#include "Shader.h"

std::string Shader::LoadShaderFile(const std::string& filePath)
{
	std::ifstream file(filePath);
	if (!file.is_open()) {
		std::cerr << "Failed to open shader file: " << filePath << std::endl;
		return "";
	}

	std::stringstream buffer;
	buffer << file.rdbuf();
	file.close();
	return buffer.str();
}

Shader::Shader(const char* vertexShaderPath, const char* fragmentShaderPath, const char* tessControlShaderPath, const char* tessEvalShaderPath)
{
	this->vertexShaderPath = vertexShaderPath;
	this->fragmentShaderPath = fragmentShaderPath;
	this->tessControlShaderPath = tessControlShaderPath;
	this->tessEvalShaderPath = tessEvalShaderPath;
	vertexShaderID = 0;
	fragmentShaderID = 0;
	tessControlShaderID = 0;
	tessEvalShaderID = 0;
	shaderID = 0;
}

Shader::~Shader()
{
	if (vertexShaderID) {
		glDetachShader(shaderID, vertexShaderID);
		glDeleteShader(vertexShaderID);
		vertexShaderID = 0;
	}
	if (fragmentShaderID) {
		glDetachShader(shaderID, fragmentShaderID);
		glDeleteShader(fragmentShaderID);
		fragmentShaderID = 0;
	}
	if (tessControlShaderID) {
		glDetachShader(shaderID, tessControlShaderID);
		glDeleteShader(tessControlShaderID);
		tessControlShaderID = 0;
	}
	if (tessEvalShaderID) {
		glDetachShader(shaderID, tessEvalShaderID);
		glDeleteShader(tessEvalShaderID);
		tessEvalShaderID = 0;
	}
	if (shaderID) {
		glDeleteProgram(shaderID);
		shaderID = 0;
	}
}

// Function to load and compile shaders, link them into a shader program
int Shader::CreateShaderProgram() { 

	std::string vertexSourceStr = LoadShaderFile(vertexShaderPath);
	const char* vertexSource = vertexSourceStr.c_str();

	std::string fragmentSourceStr = LoadShaderFile(fragmentShaderPath);
	const char* fragmentSource = fragmentSourceStr.c_str();

	// Check if shader files were loaded successfully
	if (!vertexSource || !fragmentSource) {
		std::cerr << "Error loading shader files: " << vertexShaderPath << " or " << fragmentShaderPath << std::endl;
		return -15; // Error loading shader files
	}

	vertexShaderID = glCreateShader(GL_VERTEX_SHADER); // Vertex shader ID
	fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER); // Fragment shader ID

	if (tessControlShaderPath) {
		std::string tessControlSourceStr = LoadShaderFile(tessControlShaderPath);
		const char* tessControlSource = tessControlSourceStr.c_str();

		std::string tessEvalSourceStr = LoadShaderFile(tessEvalShaderPath);
		const char* tessEvalSource = tessControlSourceStr.c_str();

		if (tessControlShaderPath && tessEvalShaderPath) { // Not required shaders but if provided, check them
			if (!tessControlSource || !tessEvalSource) {
				std::cerr << "Error loading tessellation shader files: " << tessControlShaderPath << " or " << tessEvalShaderPath << std::endl;
				return -15; // Error loading tessellation shader files
			}
			tessControlShaderID = glCreateShader(GL_TESS_CONTROL_SHADER); // Tessellation control shader ID
			tessEvalShaderID = glCreateShader(GL_TESS_EVALUATION_SHADER); // Tessellation evaluation shader ID
			glShaderSource(tessControlShaderID, 1, &tessControlSource, nullptr);
			glShaderSource(tessEvalShaderID, 1, &tessEvalSource, nullptr);
		}
	}

	glShaderSource(vertexShaderID, 1, &vertexSource, nullptr);
	glShaderSource(fragmentShaderID, 1, &fragmentSource, nullptr);

	shaderID = glCreateProgram(); // Shader program ID

	GLint status = 0;
	// Compile shaders
	glCompileShader(vertexShaderID);
	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) {
		GLint logLength = 0;
		glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &logLength);
		std::string errorLog(logLength, ' ');
		glGetShaderInfoLog(vertexShaderID, logLength, &logLength, &errorLog[0]);
		std::cerr << "Vertex shader compilation failed: " << errorLog << std::endl;
		return -16; // Vertex shader compilation error
	}
	glCompileShader(fragmentShaderID);
	glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) {
		GLint logLength = 0;
		glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &logLength);
		std::string errorLog(logLength, ' ');
		glGetShaderInfoLog(fragmentShaderID, logLength, &logLength, &errorLog[0]);
		std::cerr << "Fragment shader compilation failed: " << errorLog << std::endl;
		return -16; // Fragment shader compilation error
	}
	// Optional tessellation shaders
	if (tessControlShaderPath && tessEvalShaderPath) {
		glCompileShader(tessControlShaderID);
		glGetShaderiv(tessControlShaderID, GL_COMPILE_STATUS, &status);
		if (status == GL_FALSE) {
			GLint logLength = 0;
			glGetShaderiv(tessControlShaderID, GL_INFO_LOG_LENGTH, &logLength);
			std::string errorLog(logLength, ' ');
			glGetShaderInfoLog(tessControlShaderID, logLength, &logLength, &errorLog[0]);
			std::cerr << "Tessellation control shader compilation failed: " << errorLog << std::endl;
			return -16; // Tessellation control shader compilation error
		}
		glCompileShader(tessEvalShaderID);
		glGetShaderiv(tessEvalShaderID, GL_COMPILE_STATUS, &status);
		if (status == GL_FALSE) {
			GLint logLength = 0;
			glGetShaderiv(tessEvalShaderID, GL_INFO_LOG_LENGTH, &logLength);
			std::string errorLog(logLength, ' ');
			glGetShaderInfoLog(tessEvalShaderID, logLength, &logLength, &errorLog[0]);
			std::cerr << "Tessellation evaluation shader compilation failed: " << errorLog << std::endl;
			return -16; // Tessellation evaluation shader compilation error
		}
	}

	glAttachShader(shaderID, vertexShaderID);
	glAttachShader(shaderID, fragmentShaderID);
	if (tessControlShaderPath && tessEvalShaderPath) {
		glAttachShader(shaderID, tessControlShaderID);
		glAttachShader(shaderID, tessEvalShaderID);
	}
	// Link the shader program
	glLinkProgram(shaderID);
	glGetProgramiv(shaderID, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) {
		GLint logLength = 0;
		glGetProgramiv(shaderID, GL_INFO_LOG_LENGTH, &logLength);
		std::string errorLog(logLength, ' ');
		glGetProgramInfoLog(shaderID, logLength, &logLength, &errorLog[0]);
		std::cerr << "Shader program linking failed: " << errorLog << std::endl;
		return -17; // Shader program linking error
	}

	glValidateProgram(shaderID);

	return 0;
}

GLuint Shader::GetUniformID(std::string name) {
	auto id = uniMap.find(name);
	if (id == uniMap.end()) {
		return -1;
	}
	return id->second;
}

void Shader::Use() const
{
	glUseProgram(shaderID);
}

void Shader::setVec4(const std::string& name, const glm::vec4& value) const
{
	GLuint loc = glGetUniformLocation(shaderID, name.c_str());
	if (loc != -1) {
		glUniform4fv(loc, 1, glm::value_ptr(value));
	} else {
		std::cerr << "Warning: Uniform '" << name << "' not found in shader program." << std::endl;
	}
}

void Shader::setMat4(const std::string& name, const glm::mat4& value) const
{
	GLuint loc = glGetUniformLocation(shaderID, name.c_str());
	if (loc != -1) {
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
	} else {
		std::cerr << "Warning: Uniform '" << name << "' not found in shader program." << std::endl;
	}
}