#include "Shader.hpp"
#include "utils/File.cpp"
#include <cassert>
#include <cstdio>
#include <glad/gl.h>
#include <SDL2/SDL.h>

namespace FG24 {
bool Shader::CompileShader(const char* vertPath, const char* fragPath) {
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
		return false;
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
		return false;
	}
	
	program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, 512, nullptr, infoLog);
		std::fprintf(stderr, "Error: Renderer failed to link triangle shader program\n");
		std::fprintf(stderr, "%s\n", infoLog);
		return false;
	}

	std::printf("Program on init = %u\n", program);

	// TODO: Utilize RAII, C++ is not C
	delete vertexShaderSource;
	delete fragmentShaderSource;
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return true;
}

std::uint32_t Shader::GetShaderProgram() const {
	return program;
}

} // namespace FG24
