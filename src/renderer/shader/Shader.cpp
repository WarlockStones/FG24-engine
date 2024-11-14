#include "Shader.hpp"
#include <glad/gl.h> // Includes OpenGL
#include <glm/glm.hpp>
#include <cstdio>
#include "../../utils/files.hpp"


namespace FG24 {
Shader::Shader(const char* vertexPath, const char* fragmentPath) {
	const char* vertexShaderCode = LoadTextFile(vertexPath);
	const char* fragmentShaderCode = LoadTextFile(fragmentPath);
		
	if (vertexShaderCode && fragmentShaderCode) {
	}
}

Shader::~Shader() {
}

void Shader::Use() {
}

} // namespace FG24
