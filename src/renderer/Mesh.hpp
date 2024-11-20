#pragma once
#include <cstdint>
#include <cstddef>

namespace FG24 {
class Mesh {
public:
	// For simple triangle without any indices
	Mesh(const float* vertices, std::size_t vertexSize); 

	// For drawing a square using indices and EBO
	Mesh(const float* vertices, std::size_t vertexSize, const std::uint32_t* indices, std::size_t indicesSize); 


	void Draw(std::uint32_t shaderProgram);
private:
	std::uint32_t VBO; // Stores verticies
	std::uint32_t VAO; // Stores vertex attributes
	std::uint32_t EBO; // Stores indicies
};
}
