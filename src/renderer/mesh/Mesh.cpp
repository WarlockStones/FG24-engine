// Copyright 2024 Changemaker Educations AB. All Rights Reserved
#include "Mesh.hpp"
#include "../shader/Shader.hpp"
#include <glad/gl.h>
#include <SDL2/SDL_opengl.h>

#include <cassert>

namespace FG24 {
Mesh::Mesh(const float* vertices, std::size_t vertexArraySize, const std::uint32_t* indicies, std::size_t indexArraySize) {
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	if (indicies && indexArraySize > 0) {
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexArraySize, indicies, GL_STATIC_DRAW);
	}

	// TODO: Do not hardcode the stride. It depends on model imported. 
	// Now this is only for temporary cube

	// Vertex position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
						reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(0);

	// Texture coordinates
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
						reinterpret_cast<void*>(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Bind to null
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Mesh::Render(const Shader& shader) {
	 shader.Use();

	glBindVertexArray(VAO);
	if (EBO == 0) {
		glDrawArrays(GL_TRIANGLES, 0, 3);
	} else {
									// TODO: Fix hard coded to draw something not just cube
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	}

	glBindVertexArray(0); // Unbind
}


Mesh::~Mesh() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}
}
