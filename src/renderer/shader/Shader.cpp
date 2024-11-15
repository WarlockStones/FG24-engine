#include "Shader.hpp"
#include "../../utils/files.hpp"
#include <glad/gl.h> // Includes OpenGL
#include <glm/gtc/type_ptr.hpp>
#include <cstdio>


namespace FG24 {
Shader::Shader(const char* vertexPath, const char* fragmentPath) {
	const char* vertexShaderCode = LoadTextFile(vertexPath);
	const char* fragmentShaderCode = LoadTextFile(fragmentPath);
		
	if (vertexShaderCode && fragmentShaderCode) {
		int success{};
		char infoLog[512]{};

		std::uint32_t vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vertexShaderCode, nullptr);
		glCompileShader(vertex);
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
			std::fprintf(stderr, "Error compiling vertex shader: '%s'", vertexPath);
			// Handle error. Swap to default engine shader
		}

		std::uint32_t fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fragmentShaderCode, nullptr);
		glCompileShader(fragment);
		glGetShaderiv(fragment , GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fragment , 512, nullptr, infoLog);
			std::fprintf(stderr, "Error compiling fragment shader: '%s'", fragmentPath);
			// Handle error. Swap to default engine shader
		}

		// Create the shader program
		id = glCreateProgram();
		glAttachShader(id, vertex);
		glAttachShader(id, fragment);
		glLinkProgram(id);
		glGetProgramiv(id, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(id, 512, nullptr, infoLog);
			std::fprintf(stderr, "Error creating shader program: '%s'", fragmentPath);
		}

		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	// RAII???
	delete vertexShaderCode;
	delete fragmentShaderCode;
} 

Shader::~Shader() {
	glDeleteProgram(id);
}

void Shader::Use() const {
	glUseProgram(id);
}

void Shader::SetBool(const char* name, bool value) const {
	glUniform1i(glGetUniformLocation(id, name), static_cast<int>(value));
}

void Shader::SetInt(const char* name, int value) const {
	glUniform1i(glGetUniformLocation(id, name), value);
}

void Shader::SetFloat(const char* name, float value) const {
	glUniform1f(glGetUniformLocation(id, name), value);
}

void Shader::SetVec3(const char* name, float x, float y, float z) const {
	glUniform3f(glGetUniformLocation(id, name), x, y, z);
}

void Shader::SetMat4(const char* name, glm::mat4 value) const {
	glUniformMatrix4fv(glGetUniformLocation(id, name),
		1,						// Amount of matrices being sent
		GL_FALSE,				// Should matrix be transposed?
		glm::value_ptr(value));
}


} // namespace FG24
