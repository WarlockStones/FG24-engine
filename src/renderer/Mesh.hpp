#pragma once
#include <cstdint>
#include <cstddef>

namespace FG24 {
class Mesh {
public:
	Mesh(const float* vertices, std::size_t vertexSize);

	void Draw(std::uint32_t shaderProgram);
private:
	std::uint32_t VBO;
	std::uint32_t VAO;
};
}