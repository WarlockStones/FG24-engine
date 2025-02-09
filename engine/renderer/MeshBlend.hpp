#pragma once
#include <cstdint>
#include <cstddef>
#include <string_view>
#include <string>
#include "renderer/VertexData.hpp"

namespace FG24 {
// Stores two meshes One original, and one that is the mesh it blends to
class MeshBlend {
public:
    MeshBlend() = default;
    void Init(std::string_view name, const VertexData& data1, const VertexData& data2);
    void Draw(bool asWireframe) const;

	float m_blendAmount = 0; // Blend from one mesh to the other. from 0 to 1.
    std::string m_name;
private:
	std::size_t m_numVertices = 0;
	// OpenGL IDs
	std::uint32_t m_VBO{}; // Vertices
	std::uint32_t m_VAO{}; // Vertex attributes
  
};
} // namespace FG24
