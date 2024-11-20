#include "Triangle.hpp"
#include "Mesh.hpp"
#include <cstdio>

namespace FG24 {
static float vertices[]{
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
};

static float vertSq[] {
	 0.5f,  0.5f, 0.0f,  // Top right
	 0.5f, -0.5f, 0.0f,  // Bottom right
	-0.5f, -0.5f, 0.0f,  // Bottom left
	-0.5f,  0.5f, 0.0f   // Top left 
};

std::uint32_t indicesSq[] = {  
	0, 1, 3,   // First triangle
	1, 2, 3    // Second triangle
};  


Triangle::Triangle() : Mesh(vertices, sizeof(vertices)) {
}

Square::Square() : Mesh(vertSq, sizeof(vertSq), indicesSq, sizeof(indicesSq)) {
}

} // namespace FG24
