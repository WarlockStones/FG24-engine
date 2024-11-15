// Copyright 2024 Changemaker Educations AB. All Rights Reserved
#pragma once
#include <SDL2/SDL.h>
#include <cstddef>
#include <cstdint>


namespace FG24 {
class Shader;

class Mesh {
public:
	Mesh(const float* vertices, std::size_t vertexArraySize, const std::uint32_t* indicies, std::size_t indexArraySize);
	void Render(const Shader& shader);
	~Mesh();
	
private:
	// VBO, VAO, EBO
	std::uint32_t VBO{};
	std::uint32_t VAO{};
	std::uint32_t EBO{};
	
};
}