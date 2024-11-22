// Temporary things for when developing features of the Renderer.
// These things are here to not clutter the overall intended structure.
#include "Renderer_temp.hpp"
#include <cassert>
#include <cstdio>
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SDL2/SDL.h>
#include "Globals.hpp"
#include "Mesh.hpp"
#include "Triangle.hpp"
#include "utils/File.hpp"

namespace FG24 {
std::uint32_t TempRenderObject::shaderProgram;
Triangle* TempRenderObject::triangle{};
Square* TempRenderObject::square{};

std::uint32_t TempRenderObject::CompileSimpleShader() {
	/*
	constexpr char *vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";
	*/
	const char* vertexShaderSource = LoadTextFile("../assets/shaders/simple.vert");
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

	// Fragment shader
	/*
	const char *fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\n\0";
	*/
	const char* fragmentShaderSource = LoadTextFile("../assets/shaders/simple.frag");
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
	
	std::uint32_t program = glCreateProgram();
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

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return program;
}

void TempRenderObject::Init() {
	triangle = new Triangle();
	assert(triangle);
	square = new Square();
	assert(square);
	
	shaderProgram = CompileSimpleShader();
}

void TempRenderObject::Draw(RenderType typeToDraw) {
	assert(triangle);
	assert(square);
	assert(shaderProgram);
	
	if (typeToDraw == RenderType::triangle) {
		triangle->Draw(shaderProgram);
	} else {
		square->Draw(shaderProgram);
	}
}

} // namespace FG24

