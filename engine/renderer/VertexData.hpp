#pragma once

struct VertexData {
	float* m_data = nullptr; // Continuous array of positions, uvs, normals
	std::size_t m_numVertexData{};
	std::size_t m_numVertices{};
};
