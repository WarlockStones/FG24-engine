#pragma once
#include <cstdint>
#include <cstddef>
#include <string_view>
#include "utils/Vec.hpp"

namespace FG24 {

class Mesh {
public:
	Mesh(std::string_view name);
	void InitBuffers(
		const float* vertexData, std::size_t numVertexData,	std::size_t numVertices);

	void Draw(std::uint32_t shaderID, bool asWireframe) const;

	const std::string_view m_name;
private:
	float* vertexData = nullptr;
	std::size_t numVertexData = 0;
	std::size_t numVerticesToDraw = 0; // Used for OpenGL draw array calls
	std::size_t numVertexIndices = 0; // Used later for OpenGL draw calls

	// OpenGL IDs
	std::uint32_t VBO{}; // Stores vertices
	std::uint32_t VAO{}; // Stores vertex attributes
	std::uint32_t EBO{}; // Stores indicies
};

} // namespace FG24
