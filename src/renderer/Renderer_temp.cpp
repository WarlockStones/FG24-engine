// Temporary things for when developing features of the Renderer.
// These things are here to not clutter the overall intended structure.
#pragma once
#include <cassert>
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SDL2/SDL.h>
#include "Globals.hpp"
#include "Mesh.hpp"
#include "Triangle.hpp"

namespace FG24 {
namespace Temp {
bool CompileShaderForTriangle() {
	constexpr char *vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";

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
	const char *fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\n\0";

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
	
	g_triangleShaderProgram = glCreateProgram();
	glAttachShader(g_triangleShaderProgram, vertexShader);
	glAttachShader(g_triangleShaderProgram, fragmentShader);
	glLinkProgram(g_triangleShaderProgram);
	glGetProgramiv(g_triangleShaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(g_triangleShaderProgram, 512, nullptr, infoLog);
		std::fprintf(stderr, "Error: Renderer failed to link triangle shader program\n");
		std::fprintf(stderr, "%s\n", infoLog);
		return false;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return true;
}

void InitTriangle() {
	g_Triangle = new Triangle();
}

void DrawTriangle() {
	assert(g_Triangle);
	g_Triangle->Draw(g_triangleShaderProgram);
}
} // namespace Temp
} // namespace FG24

