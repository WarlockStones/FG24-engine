#include "Mesh.hpp"
#include <glad/gl.h>
#include <SDL2/SDL_opengl.h>
#include <cassert>

#include <cstdio> // Debug print
namespace FG24 {

Mesh::Mesh(const float* vertices, std::size_t vertexSize) {
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertexSize, vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	// Vertex attributes for triangle only
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
		reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(0);
}

Mesh::Mesh(const float* vertices, std::size_t vertexSize, const std::uint32_t* indices,
	std::size_t indicesSize) {
	std::printf("vertexSize: %lu. indiciesSize: %lu\n", vertexSize, indicesSize);
	for (std::size_t i = 0; i < vertexSize / sizeof(vertices); ++i) {
		std::printf("%f ", vertices[i]);
	}
	std::printf("\nindices:...\n");
	for (std::size_t i = 0; i < indicesSize / sizeof(indices); ++i) {
		std::printf("%u ", indices[i]);
	}
	std::printf("\n");


	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertexSize, vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Vertex attributes for textured square only
	
	// Vertex position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
						  reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(0);

	// Vertex Color. Intentionally not implemented for now
	// glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float),
	//					  reinterpret_cast<void *>(3 * sizeof(float)));


	// Texture coordinates (aka. UV)
	// TODO: Enable after testing MeshData bug
	/*
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
						  reinterpret_cast<void*>(6 * sizeof(float)));
	glEnableVertexAttribArray(1);
	*/

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, GL_STATIC_DRAW);
}

Mesh::Mesh(MeshData data) {
	auto numVertices = data.numVertices;
	std::printf("MeshData num vertices: %lu\n", numVertices);
	// numIndicies = data.numFaces. // TODO: Get num indicies not number of faces!
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	std::printf("MeshData.......\n");
	for (std::size_t i = 0; i < numVertices; ++i) {
		std::printf("%f %f %f", data.vertices[i].x, data.vertices[i].y, data.vertices[i].z);
	}
	std::printf("\n");
	std::printf("Indices: \n");
	for (std::size_t i = 0; i < data.numIndices; ++i) {
	  std::printf("%u ", data.indices[i]);
	}
	std::printf("\n");

	// Vert size became 32, should be 128?
	// OBS! Do not do sizeof (data.indices) instead do what the type of indices is!
	auto vertSize = sizeof(float) * 3 * numVertices; // 
	auto indSize = sizeof(std::uint32_t) * data.numIndices; //
	std::printf("MeshData: vertexSize: %lu. indiciesSize: %lu\n", vertSize, indSize);
	// TODO: do not hard-code 128!
	glBufferData(GL_ARRAY_BUFFER, vertSize, data.vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	// Vertex position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
						  reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(std::uint32_t) * data.numIndices, data.indices, GL_STATIC_DRAW);
}

std::uint32_t Mesh::GetVBO() const {
	return VBO;
}
std::uint32_t Mesh::GetVAO() const {
	return VAO;
}
std::uint32_t Mesh::GetEBO() const {
	return EBO;
}
} // namespace FG24
