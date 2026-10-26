#pragma once
#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>
#include <glm.hpp>
#include <fast_obj.h>
#include <vector>
#include <string>
#include <iostream>
#include "tiny_obj_loader.h"
#include "Texture.h"

// Use assimp in future for more model support

class Mesh {
private:

	struct Vertex {
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 uv;
		int materialId;
	};

	std::vector<Vertex> vertexData;
	std::vector<tinyobj::material_t> materials;

	GLuint vao, vbo;
	GLsizei vertexCount = 0;

public:
	std::vector<Texture*> loadedTextures;

	Mesh();
	~Mesh();

	void CreateMesh(const char* file); // CPU sided
	void MeshToGPU(); // GPU sided
	void RenderMesh() const;
	void DestroyMesh();

	// Getters
	GLuint GetVAO() const { return vao; }
	GLsizei GetVertexCount() const { return vertexCount; }
};

#endif // !MESH_H
