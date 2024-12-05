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
	float x, y, z;
};

// A face built from three vertex indices
struct Face {
	// An obj face can contain three or more vertices. 3=normal tri, 4=quads, More than that = n-gon.
	// OpenGL likes triangles, So I am only going to support faces that are triangles.
	std::uint32_t a[3], b[3], c[3]; 
};

// Collection of pointers to dynamically allocated arrays of vertices, UVs, normals, and faces
struct MeshData {
	// TODO: Arrays of Vertices, UVs, Normals, and Faces.
	// OBJ: v = vertex, vn = vertexnormal, vt = vertextexture(uv), f = face/index

	// Owning pointers. Should probably make them std::unique_ptr
	Vertex* vertices = nullptr;
	UV* UVs = nullptr;
	Normal* normals = nullptr;
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
private:
	std::uint32_t VBO{}; // Stores verticies
	std::uint32_t VAO{}; // Stores vertex attributes
	std::uint32_t EBO{}; // Stores indicies
};
}
