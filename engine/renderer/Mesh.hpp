#pragma once
#include <cstdint>
#include <cstddef>

namespace FG24 {

// Vertex, UV, Normal, Face, and MeshData must be as simple as possible because
// they will be stored on disk and read.
struct Vertex {
	float x, y, z;
};

struct UV {
	float U, V;
};

struct Normal {
	float i, j, k;
};

// Collection of pointers to dynamically allocated arrays of vertices, UVs, normals, and faces
struct MeshData {
	// Owning pointers. Should probably make them std::unique_ptr
	std::size_t numVertices{};
	Vertex* vertices = nullptr;

	std::size_t numUVs{};
	UV* UVs = nullptr;

	std::size_t numNormals{};
	Normal* normals = nullptr;

	// OpenGL wants all vertices AND face-index-grouping values as continous arrays
	std::size_t numVertexIndices = 0;
	std::int32_t* vertexIndices = nullptr;

	std::size_t numUVIndices = 0;
	std::int32_t* UVIndices = nullptr;
  
	std::size_t numNormalIndices = 0;
	std::int32_t* normalIndices = nullptr;
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
