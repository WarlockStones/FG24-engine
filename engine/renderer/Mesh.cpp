#include "Mesh.hpp"
#include <glad/gl.h>
#include <SDL2/SDL_opengl.h>
#include <cassert>

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
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
						  reinterpret_cast<void*>(6 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, GL_STATIC_DRAW);

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
