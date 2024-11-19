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

	// Vertex attributes for triangle
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
		reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(0);
}

void Mesh::Draw(std::uint32_t shaderProgram) {
	assert(VAO);
	assert(VBO);

	// Draw triangle which is just an array of a single vertex attribute pointer
	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	// glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // It is not an EBO yet, we are only drawing an array of vertex position (a single vertex attribute)
	glBindVertexArray(0);
}

} // namespace FG24