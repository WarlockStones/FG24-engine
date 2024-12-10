#include "SimpleShapes.hpp"
#include "Mesh.hpp"
#include <cstdio>

namespace FG24 {
float vertices[]{
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
};

float vertSq[] = {
	// positions          // colors           // texture coords
	 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
	 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
	-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
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
