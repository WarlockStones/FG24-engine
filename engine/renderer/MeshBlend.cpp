#include "MeshBlend.hpp"
#include <glad/gl.h>
#include <SDL2/SDL_opengl.h>
#include <cassert>
#include <string_view>
#include "renderer/VertexData.hpp"

#include <cstdlib>

namespace FG24 {
void MeshBlend::Init(
	std::string_view name,
	const VertexData& data1,
	const VertexData& data2) {

	assert(data1.m_data != nullptr);
	assert(data1.m_numVertices > 0);
	assert(data2.m_data != nullptr);
	assert(data2.m_numVertices > 0);
	
	assert(data1.m_numVertices == data2.m_numVertices); // Can only blend of equal size

	m_name = name;
	
	m_numVertices = data1.m_numVertices;

	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

	std::size_t dataSize = sizeof(float) * data1.m_numVertexData;
	glBufferData(GL_ARRAY_BUFFER, dataSize * 2, nullptr, GL_STATIC_DRAW);

	// Load data
	glBufferSubData(GL_ARRAY_BUFFER, 0,        dataSize, data1.m_data);
	glBufferSubData(GL_ARRAY_BUFFER, dataSize, dataSize, data2.m_data);
	
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	// Position
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


	// Position 2
	glVertexAttribPointer(
		3, 
		3,
		GL_FLOAT, 
		GL_FALSE,
		8 * sizeof(float), 
		reinterpret_cast<void*>(dataSize));

	// UV 2
	glVertexAttribPointer(
		4, 
		2, 
		GL_FLOAT, 
		GL_FALSE, 
		8 * sizeof(float), 
		reinterpret_cast<void*>(dataSize + (3 * sizeof(float)))); 

	// Normal 2
	glVertexAttribPointer(
		5, 
		3, 
		GL_FLOAT, 
		GL_FALSE, 
		8 * sizeof(float), 
		reinterpret_cast<void*>(dataSize + (5 * sizeof(float))));
	
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glEnableVertexAttribArray(5);
}

void MeshBlend::Draw(bool asWireframe = false) const {
	// assert(shaderId != 0); // BlendShader
	std::printf("IMPLEMENT MeshBlend Draw!\n");
    std::abort();

	if (asWireframe) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Wireframe
	} else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Full polygons
	}

	// glUseProgram(shaderId); // Use Blend shader
	glBindVertexArray(m_VAO);

	glDrawArrays(GL_TRIANGLES, 0, m_numVertices); 

	// Send blend amount uniform
	
	glBindVertexArray(0); // Unbind
}
  

} // namespace FG24
