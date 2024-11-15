// Copyright 2024 Changemaker Educations AB. All Rights Reserved
#pragma once
#include <cstdint>
#include <glm/ext/matrix_float4x4.hpp>

namespace FG24 {
class Shader {
public:
	// Input is path to vertex and fragment shader files
	Shader(const char* vertexPath, const char* fragmentPath);
	~Shader();

	void Use() const;

	void SetBool(const char* name, bool value) const;
	void SetInt(const char* name, int value) const;
	void SetFloat(const char* name, float value) const;
	void SetVec3(const char* name, float x, float y, float z) const;
	void SetMat4(const char* name, glm::mat4) const;
private:
	std::uint32_t id{}; // Shader program ID;
};
}
