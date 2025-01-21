#include "Shader.hpp"
#include "utils/File.hpp"
#include <cassert>
#include <cstdio>
#include <glad/gl.h>
#include <SDL2/SDL.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

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
		std::fprintf(stderr, "Error: Renderer failed to compile triangle vertex m_shaderID\n");
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
		std::fprintf(stderr, "Error: Renderer failed to compile triangle fragment m_shaderID\n");
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
		std::fprintf(stderr, "Error: Renderer failed to link triangle m_shaderID program\n");
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
void Use(std::uint32_t shaderID) {
  glUseProgram(shaderID);
}
void SetInt(std::uint32_t shaderID, const char* name, int value) {
	glUniform1i(glGetUniformLocation(shaderID, name), value);
}
void SetFloat(std::uint32_t shaderID, const char* name, float value) {
	glUniform1f(glGetUniformLocation(shaderID, name), value);

}
void SetVec3 (std::uint32_t shaderID, const char* name, glm::vec3 value) {
	glUniform3f(glGetUniformLocation(shaderID, name),
				value.x,
				value.y,
				value.z);
}
void SetMat4 (std::uint32_t shaderID, const char* name, glm::mat4 value) {
	glUniformMatrix4fv(glGetUniformLocation(shaderID, name),
					  1, // Amout of matrices we are sending
					  GL_FALSE, // Should the matrix be transposed?
					  glm::value_ptr(value)); // Pointer to transformation value

}


} // namespace Shader
} // namespace FG24
