#pragma once
#include <cstdint>
#include <cstddef>
#include <string_view>
#include "utils/Vec.hpp"
#include "renderer/VertexData.hpp"

namespace FG24 {

class Mesh {
public:
	Mesh(std::string_view name);
	void InitBuffers(VertexData& vertexData);
	void Draw(std::uint32_t shaderID, bool asWireframe) const;

	const std::string_view m_name;
private:
	std::size_t numVerticesToDraw = 0; // Used for OpenGL draw array calls

	// OpenGL IDs
	std::uint32_t VBO{}; // Stores vertices
	std::uint32_t VAO{}; // Stores vertex attributes
	std::uint32_t EBO{}; // Stores indicies
};

} // namespace FG24
