#pragma once
#include <cstdint>

struct VertexData {
	float* m_data = nullptr; // Continuous array of positions, uvs, normals
	std::uint32_t m_numVertexData{};
	std::uint32_t m_numVertices{};
};
