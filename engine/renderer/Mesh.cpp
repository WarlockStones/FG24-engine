#include "Mesh.hpp"
#include <glad/gl.h>
#include <SDL2/SDL_opengl.h>
#include <cassert>

namespace FG24 {

void Mesh::InitBuffers(
	const float* vertexData,
	std::size_t numVertexData,
	std::size_t numVerticies)
{
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	numVerticiesToDraw = numVerticies;
  
	assert(vertexData != nullptr);
	glBufferData(
		GL_ARRAY_BUFFER,                    // Type
		sizeof(float) * numVertexData, // Size in bytes of buffer
		vertexData,                    // Pointer to data to be copied
		GL_STATIC_DRAW);                    // Usage pattern
	
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Vertex Position
	glVertexAttribPointer(
		0, // index of stream of data for m_shaderID [[ layout (location = 0) in  ]]
		3, // num components. xyz = 3 
		GL_FLOAT, // Type
		GL_FALSE, // Normalize
		8 * sizeof(float), // Stride. Size in bytes until next consecutive attrib
		reinterpret_cast<void*>(0)); // Starting position of this type

	// UV
	glVertexAttribPointer(
		1, 
		2, 
		GL_FLOAT, 
		GL_FALSE, 
		8 * sizeof(float), 
		reinterpret_cast<void*>(3 * sizeof(float))); 

	// Normal
	glVertexAttribPointer(
		2, 
		3, 
		GL_FLOAT, 
		GL_FALSE, 
		8 * sizeof(float), 
		reinterpret_cast<void*>(5 * sizeof(float)));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
}

void Mesh::Draw(std::uint32_t shaderID) const {
	assert(shaderID != 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Full polygons
	glUseProgram(shaderID);

	glBindVertexArray(VAO);

	if (numVertexIndices > 0) { // MeshData
		glDrawElements(GL_TRIANGLES, numVertexIndices, GL_UNSIGNED_INT, 0);  
	}
	else if (EBO > 0) { // Just an EBO but nothing from MeshData
		// Draw the square. It has an EBO and 6 vertices
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);  
	} else if (EBO == 0) {
		// glDrawArrays(GL_TRIANGLES, 0, 3); // Draw triangle which has only 3 vertices
		glDrawArrays(GL_TRIANGLES, 0, numVerticiesToDraw); 
	}
	
	glBindVertexArray(0); // Unbind
}
} // namespace FG24
