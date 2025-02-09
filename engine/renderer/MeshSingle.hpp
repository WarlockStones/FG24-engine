#pragma once
#include <cstdint>
#include <cstddef>
#include <string_view>
#include <string>
#include "renderer/VertexData.hpp"

namespace FG24 {

class MeshSingle {
public:
	void Init(std::string_view name, const VertexData& vertexData);
	void Draw(std::uint32_t shaderID, bool asWireframe) const;

	std::string m_name{};
private:
	std::size_t numVerticesToDraw = 0; // Used for OpenGL draw array calls

	// OpenGL IDs
	std::uint32_t VBO{}; // Stores vertices
	std::uint32_t VAO{}; // Stores vertex attributes
	std::uint32_t EBO{}; // Stores indicies
};

} // namespace FG24
