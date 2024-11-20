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
	// Vertex attributes for square only
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
						  reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, GL_STATIC_DRAW);

}
void Mesh::Draw(std::uint32_t shaderProgram) {
	assert(VAO);
	assert(VBO);

	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);
	if (EBO > 0) {
		// Draw the square. It has an EBO and 6 verticies
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);  
	} else {
		// Draw the triangle. it has no EBO and only 3 verticies
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
	
	glBindVertexArray(0); // Unbind
}

} // namespace FG24
