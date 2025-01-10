#pragma once
#include <cstdint>
#include <cstddef>
#include "utils/Vec.hpp"

namespace FG24 {

// Collection of pointers to dynamically allocated arrays of vertices, UVs, normals, and faces
struct MeshData {
	// Owning pointers. Should probably make them std::unique_ptr
	std::size_t numVertexPositions{};
	Vec3* vertexPositions = nullptr;

	std::size_t numUVs{};
	Vec2* UVs = nullptr;

	std::size_t numNormals{};
	Vec3* normals = nullptr;

	// OpenGL wants all vertices AND face-index-grouping values as continuous arrays
	std::size_t numVertexIndices = 0;
	std::int32_t* vertexIndices = nullptr;
};

class Mesh {
public:
	// For simple triangle without any indices
	Mesh(const float* vertices, std::size_t vertexSize);

	// For drawing a square using indices and EBO
	Mesh(const float* vertices, std::size_t vertexSize, const std::uint32_t* indices, std::size_t indicesSize);

	// For mesh data loaded from file
	Mesh(const MeshData& data);

	void Draw(std::uint32_t shaderID);

	std::size_t numVertexIndices = 0; // Used later for OpenGL draw calls
private:
	// OpenGL IDs
	std::uint32_t VBO{}; // Stores verticies
	std::uint32_t VAO{}; // Stores vertex attributes
	std::uint32_t EBO{}; // Stores indicies
};

} // namespace FG24
