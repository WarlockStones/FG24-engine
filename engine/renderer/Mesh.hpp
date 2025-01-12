#pragma once
#include <cstdint>
#include <cstddef>
#include "utils/Vec.hpp"
#include <cstddef>

namespace FG24 {

class Mesh {
public:
    Mesh() = default;
    void InitBuffers(
		const float* vertexData, std::size_t numVertexData,	std::size_t numVerticies);

	void Draw(std::uint32_t shaderID);

private:
    float* vertexData = nullptr;
    std::size_t numVertexData = 0;
    std::size_t numVerticiesToDraw = 0; // Used for OpenGL draw array calls
	std::size_t numVertexIndices = 0; // Used later for OpenGL draw calls

	// OpenGL IDs
	std::uint32_t VBO{}; // Stores verticies
	std::uint32_t VAO{}; // Stores vertex attributes
	std::uint32_t EBO{}; // Stores indicies
};

} // namespace FG24
