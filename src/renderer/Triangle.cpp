#include "Triangle.hpp"
#include "Mesh.hpp"

namespace FG24 {
static float vertices[]{
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
};


Triangle::Triangle() : Mesh(vertices, sizeof(vertices)) {
}

} // namespace FG24
