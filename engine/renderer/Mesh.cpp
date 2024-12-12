#include "Mesh.hpp"
#include <glad/gl.h>
#include <SDL2/SDL_opengl.h>
#include <cassert>

#include <cstdio> // Debug print
namespace FG24 {

// TODO: Mesh should not be doing fancy OpenGL stuff in constructor.
// For cube in SimpleShapes.cpp TODO: Remove this
Mesh::Mesh(const float* vertices, std::size_t vertexSize) {
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertexSize, vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Vertex attributes for SimpleShape.cpp Cube only
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
		reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(0);

	// Texture coordinates (aka. UV)
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
						  reinterpret_cast<void*>(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

// VertSq in SimpleShapes.cpp. TODO: Remove this
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

Mesh::Mesh(const MeshData& data) {
	numIndices = data.numIndices;

	std::size_t vertSize = sizeof(Vertex) * data.numVertices; 
	// Do not do sizeof the member (data.indices), instead use the type
	std::size_t indSize = sizeof(std::uint32_t) * data.numIndices; // test something

#if true
	std::printf("MeshData num vertices: %lu\n", data.numVertices);
	std::printf("MeshData: vertexSize: %lu. indiciesSize: %lu\n", vertSize, indSize);
	for (std::size_t i = 0; i < data.numVertices; ++i) {
		std::printf("%f %f %f\n", data.vertices[i].x, data.vertices[i].y, data.vertices[i].z);
	}
	std::printf("\n");
	std::printf("MeshData Indices: %lu\n", data.numIndices);
	for (std::size_t i = 0; i < data.numIndices; ++i) {
	  std::printf("%u ", data.indices[i]);
	}
	std::printf("..\n");
#endif

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, vertSize, data.vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Vertex position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
						  reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indSize, data.indices, GL_STATIC_DRAW);
}

void Mesh::Draw(std::uint32_t shaderID) {
	assert(shaderID != 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Full polygons
	glUseProgram(shaderID);

	glBindVertexArray(VAO);

	if (numIndices > 0) { // MeshData
		glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);  
	}
	else if (EBO > 0) { // Just an EBO but nothing from MeshData
		// Draw the square. It has an EBO and 6 vertices
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);  
	} else if (EBO == 0) {
		// glDrawArrays(GL_TRIANGLES, 0, 3); // Draw triangle which has only 3 vertices
		glDrawArrays(GL_TRIANGLES, 0, 36); // Draw SimpleShapes.cpp Cube
	}
	
	glBindVertexArray(0); // Unbind
}
} // namespace FG24
