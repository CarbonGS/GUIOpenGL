#include "Mesh.h"
#include <glm/gtx/string_cast.hpp>

Mesh::Mesh()
{
    vao = 0;
    vbo = 0;
	vertexCount = 0;
}

Mesh::~Mesh() {
    DestroyMesh();
}

//void Mesh::CreateMesh(fastObjMesh* mesh) debugging cube mesh hardcoded
//{
//    vertices.clear();
//    normals.clear();
//    uvCoords.clear();
//
//    // Cube vertices (6 faces × 2 triangles × 3 vertices = 36)
//    std::vector<glm::vec3> cubeVertices = {
//        // Front face
//        {-0.5f, -0.5f,  0.5f}, { 0.5f, -0.5f,  0.5f}, { 0.5f,  0.5f,  0.5f},
//        {-0.5f, -0.5f,  0.5f}, { 0.5f,  0.5f,  0.5f}, {-0.5f,  0.5f,  0.5f},
//
//        // Back face
//        {-0.5f, -0.5f, -0.5f}, {-0.5f,  0.5f, -0.5f}, { 0.5f,  0.5f, -0.5f},
//        {-0.5f, -0.5f, -0.5f}, { 0.5f,  0.5f, -0.5f}, { 0.5f, -0.5f, -0.5f},
//
//        // Left face
//        {-0.5f, -0.5f, -0.5f}, {-0.5f, -0.5f,  0.5f}, {-0.5f,  0.5f,  0.5f},
//        {-0.5f, -0.5f, -0.5f}, {-0.5f,  0.5f,  0.5f}, {-0.5f,  0.5f, -0.5f},
//
//        // Right face
//        { 0.5f, -0.5f, -0.5f}, { 0.5f,  0.5f, -0.5f}, { 0.5f,  0.5f,  0.5f},
//        { 0.5f, -0.5f, -0.5f}, { 0.5f,  0.5f,  0.5f}, { 0.5f, -0.5f,  0.5f},
//
//        // Top face
//        {-0.5f,  0.5f, -0.5f}, {-0.5f,  0.5f,  0.5f}, { 0.5f,  0.5f,  0.5f},
//        {-0.5f,  0.5f, -0.5f}, { 0.5f,  0.5f,  0.5f}, { 0.5f,  0.5f, -0.5f},
//
//        // Bottom face
//        {-0.5f, -0.5f, -0.5f}, { 0.5f, -0.5f, -0.5f}, { 0.5f, -0.5f,  0.5f},
//        {-0.5f, -0.5f, -0.5f}, { 0.5f, -0.5f,  0.5f}, {-0.5f, -0.5f,  0.5f}
//    };
//
//    // Dummy normals per vertex (pointing out — you can improve per face)
//    for (int i = 0; i < cubeVertices.size(); ++i)
//        normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
//
//    // UV coordinates (simple planar mapping)
//    std::vector<glm::vec2> cubeUVs = {
//        {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f},
//        {0.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}
//    };
//
//    for (int i = 0; i < 6; ++i) {
//        uvCoords.insert(uvCoords.end(), cubeUVs.begin(), cubeUVs.end());
//    }
//
//    // Copy vertices into the class vector
//    vertices.insert(vertices.end(), cubeVertices.begin(), cubeVertices.end());
//}

void Mesh::CreateMesh(const char* file) {
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string err;

    if (!vertexData.empty()) {
        vertexData.clear();
    }
    if (!loadedTextures.empty()) {
        for (Texture* texture : loadedTextures) {
            if (texture) {
                delete texture;
            }
        }
        loadedTextures.clear();
    }

    Vertex v = {};
    
    std::string baseDir = "Meshes/"; // or extract from file path
    tinyobj::LoadObj(&attrib, &shapes, &materials, &err, file, baseDir.c_str());

    for (size_t i = 0; i < materials.size(); ++i) {
        std::string texFile = materials[i].diffuse_texname;
        if (!texFile.empty()) {
            std::string fullPath = "Meshes/" + texFile;
            Texture* t = new Texture();
            t->LoadTexture(fullPath);
            loadedTextures.push_back(t);
        }
        else {
            loadedTextures.push_back(nullptr);
        }
    }

    for (const auto& shape : shapes) {
        size_t index_offset = 0;
        for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++) {
            int fv = shape.mesh.num_face_vertices[f];
            int material_id = shape.mesh.material_ids[f];

            if (material_id < 0 || material_id >= static_cast<int>(materials.size())) {
                material_id = -1; // Invalid material
            }

            for (size_t vtx = 0; vtx < fv; vtx++) {
                tinyobj::index_t idx = shape.mesh.indices[index_offset + vtx];
                v.position = glm::vec3(
                    attrib.vertices[3 * idx.vertex_index + 0],
                    attrib.vertices[3 * idx.vertex_index + 1],
                    attrib.vertices[3 * idx.vertex_index + 2]
                );
                v.normal = (idx.normal_index >= 0)
                    ? glm::vec3(
                        attrib.normals[3 * idx.normal_index + 0],
                        attrib.normals[3 * idx.normal_index + 1],
                        attrib.normals[3 * idx.normal_index + 2]
                    )
                    : glm::vec3(0.0f, 0.0f, 1.0f);
                v.uv = (idx.texcoord_index >= 0)
                    ? glm::vec2(
                        attrib.texcoords[2 * idx.texcoord_index + 0],
                        attrib.texcoords[2 * idx.texcoord_index + 1]
                    )
                    : glm::vec2(0.0f, 0.0f);
                v.materialId = material_id;
                vertexData.push_back(v);
            }
            index_offset += fv;
        }
    }
}

void Mesh::MeshToGPU()
{
	// Generate and bind the VAO
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

	// Generate and bind the VBO
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(Vertex), vertexData.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0); // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

    glEnableVertexAttribArray(1); // normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    glEnableVertexAttribArray(2); // uv
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

    glEnableVertexAttribArray(3); // materialId
    glVertexAttribIPointer(3, 1, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, materialId));

	vertexCount = static_cast<GLsizei>(vertexData.size());
    vertexData.clear();
    glBindVertexArray(0);
}

void Mesh::RenderMesh() const
{
    glBindVertexArray(vao); // Bind the VAO
    glDrawArrays(GL_TRIANGLES, 0, vertexCount); // Draw the mesh
	glBindVertexArray(0); // Unbind the VAO
}

void Mesh::DestroyMesh()
{
    if (vao) {
        glDeleteVertexArrays(1, &vao);
    }
    if (vbo) {
        glDeleteBuffers(1, &vbo);
    }
    if (!loadedTextures.empty()) {
        for (Texture* texture : loadedTextures) {
            if (texture) {
                delete texture;
            }
        }
        loadedTextures.clear();
	}
    if (!materials.empty()) {
        materials.clear();
	}
    if (!vertexData.empty()) {
        vertexData.clear();
    }
	materials.clear();
	vertexData.clear();
    vertexCount = 0;
}
