#include "Shader.hpp"
#include "utils/File.cpp"
#include <cassert>
#include <cstdio>
#include <glad/gl.h>
#include <SDL2/SDL.h>

namespace FG24 {
namespace Shader {
std::uint32_t CompileShader(const char* vertPath, const char* fragPath) {
	const char* vertexShaderSource = File::LoadTextFile(vertPath);
	assert(vertexShaderSource);

	std::uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
		std::fprintf(stderr, "Error: Renderer failed to compile triangle vertex shader\n");
		std::fprintf(stderr, "%s\n", infoLog);
		return 0;
	}

	const char* fragmentShaderSource = File::LoadTextFile(fragPath);
	assert(fragmentShaderSource);

	std::uint32_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
		std::fprintf(stderr, "Error: Renderer failed to compile triangle fragment shader\n");
		std::fprintf(stderr, "%s\n", infoLog);
		return 0;
	}
	
	std::uint32_t program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, 512, nullptr, infoLog);
		std::fprintf(stderr, "Error: Renderer failed to link triangle shader program\n");
		std::fprintf(stderr, "%s\n", infoLog);
		return 0;
	}

	// TODO: Utilize RAII, C++ is not C
	delete vertexShaderSource;
	delete fragmentShaderSource;
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return program;
}
} // namespace Shader
} // namespace FG24
