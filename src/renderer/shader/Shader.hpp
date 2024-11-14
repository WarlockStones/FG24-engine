// Copyright 2024 Changemaker Educations AB. All Rights Reserved
#pragma once
#include <cstdint>

namespace FG24 {
class Shader {
public:
	std::uint32_t id{}; // Shader program ID;

	// Input is path to vertex and fragment shader files
	Shader(const char* vertexPath, const char* fragmentPath);
	~Shader();

	void Use();
private:
};
}
