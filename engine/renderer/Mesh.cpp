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

	// BAD NAME! This is not just vertex. It is vertex and UV in one array!
	// Or am I confused? One vertex has: vertexPOSITION, UV, Normal, etc.
	glBufferData(GL_ARRAY_BUFFER, vertexSize, vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// --- Vertex attributes for SimpleShape.cpp Cube only ---
	// Vertex position:
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
	// MeshData does not have everything in one array
	numVertexIndices = data.numVertexIndices;

	std::size_t vertSize = sizeof(Vec3) * data.numVertexPositions; 
	std::size_t UVSize = sizeof(Vec2) * data.numUVs;
	std::size_t normalSize = sizeof(Vec3) * data.numNormals;

	// Do not do sizeof the member (data.indices), use the type
	std::size_t vertIndSize = sizeof(std::uint32_t) * data.numVertexIndices; // test something

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Data will be set using glBufferSubData so it is nullptr now
	std::size_t bufferSize = vertSize + normalSize + UVSize;
	glBufferData(GL_ARRAY_BUFFER, bufferSize, nullptr, GL_STATIC_DRAW);

	// Load data
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertSize, data.vertexPositions);
	glBufferSubData(GL_ARRAY_BUFFER, vertSize, UVSize, data.UVs);
	glBufferSubData(GL_ARRAY_BUFFER, vertSize + UVSize, normalSize, data.normals);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Vertex position
	glVertexAttribPointer(
		0, // index of stream of data for shader [[ layout (location = 0) in  ]]
		3, // xyz = 3 
		GL_FLOAT, 
		GL_FALSE,
		3 * sizeof(float),
		reinterpret_cast<void*>(0));

	// UV
	glVertexAttribPointer(
		1,
		2,
		GL_FLOAT,
		GL_FALSE,
		2 * sizeof(float),
		reinterpret_cast<void*>(vertSize));
	
	// Normal
	glVertexAttribPointer(
		2,
		3,
		GL_FLOAT,
		GL_FALSE,
		3 * sizeof(float),
		reinterpret_cast<void*>(vertSize + UVSize));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertIndSize, data.vertexIndices, GL_STATIC_DRAW);
	
#if true // Debug print
	int faceNum = 0;
	for (int i = 0; i < numVertexIndices; i += 3) {
		std::printf("Face %d\n", faceNum+1);

		// Print vertex position for that face
		auto v1 = data.vertexPositions[data.vertexIndices[i]]; 
		std::printf("\tVert 1: %fx %fy %fz\n", v1.x, v1.y, v1.z);

		auto v2 = data.vertexPositions[data.vertexIndices[i+1]];
		std::printf("\tVert 2: %fx %fy %fz\n", v2.x, v2.y, v2.z);

		auto v3 = data.vertexPositions[data.vertexIndices[i+2]];
		std::printf("\tVert 3: %fx %fy %fz\n", v3.x, v3.y, v3.z);
		
		faceNum++;
	}
	std::printf("----\n");
#endif
}

void Mesh::InitBuffers(const MeshData& data) {
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
  
	glBufferData(
		GL_ARRAY_BUFFER,                        // Type
		data.numVertexPositions * sizeof(Vec3), // Size in bytes of buffer
		data.vertexPositions,                   // Pointer to data to be copied
		GL_STATIC_DRAW);                        // Usage pattern
	
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	
	// Vertex Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0,
		reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(0);
}

void Mesh::Draw(std::uint32_t shaderID) {
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
		glDrawArrays(GL_TRIANGLES, 0, 36); // Draw SimpleShapes.cpp Cube
	}
	
	glBindVertexArray(0); // Unbind
}
} // namespace FG24
