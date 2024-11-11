#include "../shader/Shader.hpp"
#include <glad/gl.h> // Includes OpenGL
#include <glm/glm.hpp>
#include <cstdio>


namespace FG24 {
Shader::Shader(const char* vertexPath, const char* fragmentPath) {
	const char* vertexShaderCode = LoadShaderFile(vertexPath);
	const char* fragmentShaderCode = LoadShaderFile(fragmentPath);
		
	if (vertexShaderCode && fragmentShaderCode) {


	}
}

Shader::~Shader() {
}

void Shader::Use() {
}

const char* Shader::LoadShaderFile(const char* path) {
	std::FILE* stream = std::fopen(path, "r");
	if (!stream)
	{
		std::fprintf(stderr, "Error: Shader failed to open path...\n \'%s\'\n", path);
		return nullptr;
	}

	

	if (std::fclose(stream)) {
		std::fprintf(stderr, "Error: Shader failed to close file stream.\n");
	}
}

} // namespace FG24
