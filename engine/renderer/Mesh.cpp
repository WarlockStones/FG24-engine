#include "Mesh.hpp"
#include <glad/gl.h>
#include <SDL2/SDL_opengl.h>
#include <cassert>

namespace FG24 {

Mesh::Mesh(std::string_view name) : m_name(name) {

}

void Mesh::InitBuffers(VertexData& vertexData) {
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	numVerticesToDraw = vertexData.m_numVertices;
  
	assert(vertexData.m_data != nullptr);
	glBufferData(
		GL_ARRAY_BUFFER,    // Type
		sizeof(float) * vertexData.m_numVertexData,  // Size in bytes of buffer
		vertexData.m_data,  // Pointer to data to be copied
		GL_STATIC_DRAW);    // Usage pattern
	
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

void Mesh::Draw(std::uint32_t shaderId, bool asWireframe = false) const {
	assert(shaderId != 0);
	if (asWireframe) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Wireframe
	} else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Full polygons
	}

	glUseProgram(shaderId);
	glBindVertexArray(VAO);

	glDrawArrays(GL_TRIANGLES, 0, numVerticesToDraw); 
	
	glBindVertexArray(0); // Unbind
}
} // namespace FG24
