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

struct Face {
	static constexpr std::size_t max = 3;
	std::size_t numIndices = 0;
	std::uint32_t v[max]{}, vt[max]{}, vn[max]{}; // Indicies
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

	std::size_t numFaces{};
	Face* faces = nullptr;
};

class Mesh {
public:
	// For simple triangle without any indices
	Mesh(const float* vertices, std::size_t vertexSize);

	// For drawing a square using indices and EBO
	Mesh(const float* vertices, std::size_t vertexSize, const std::uint32_t* indices, std::size_t indicesSize);
	Mesh(MeshData);

	std::uint32_t GetVBO() const;
	std::uint32_t GetVAO() const;
	std::uint32_t GetEBO() const;
	std::size_t numVertices = 0;
	std::size_t numIndicies = 0;
private:
	std::uint32_t VBO{}; // Stores verticies
	std::uint32_t VAO{}; // Stores vertex attributes
	std::uint32_t EBO{}; // Stores indicies
};
}
