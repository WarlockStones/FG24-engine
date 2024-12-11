#include "Renderer.hpp"
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SDL2/SDL.h>
#include <cstdio>
#include "Globals.hpp"
#include "framework/Actor.hpp"
#include "Shader.hpp"


namespace FG24 {
bool Renderer::Init() {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::fprintf(stderr, "Error: Renderer failed to initialize SDL!\n");
		std::fprintf(stderr, "%s\n", SDL_GetError());
		return false;
	}

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	uint32_t windowFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_MOUSE_GRABBED;

	window = SDL_CreateWindow("FG24 Engine", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, g_windowWidth, g_windowHeight, windowFlags);

	context = SDL_GL_CreateContext(window);

	if (!gladLoadGL(GLADloadfunc(SDL_GL_GetProcAddress))) {
		std::fprintf(stderr, "Error: Renderer failed to initialize GLAD!\n");
		return false;
	}

	// Hide cursor and constrain it to the window
	SDL_SetRelativeMouseMode(SDL_TRUE); 

	// 0 = none, 1 = vsync, -1 = adaptive vsync
	if (SDL_GL_SetSwapInterval(0) != 0) {
		std::fprintf(stderr, "Error: Renderer failed to set SDL swap interval!\n");
		std::fprintf(stderr, "%s\n", SDL_GetError());
		return false;
	}

	return true;
}

void Renderer::Draw() {
	glClearColor(0.21f, 0.21f, 0.21f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// g_triangle->Draw();

	///////////////////////////////////////////////////////////////////////////////////////
	// This should be set per model basis
	// I have not decided where to put this, so for now it is just here. Matrixes and stuff
	///////////////////////////////////////////////////////////////////////////////////////
	// Transformation
	glm::mat4 transform = glm::mat4(1.0f); // Initialize identity matrix first
	transform = glm::translate(transform, glm::vec3(0.5f, 0.5f, 0.0f)); // move position to top right
	// You must glm::translate before you glm::rotate!
	transform = glm::rotate(transform, (float)SDL_GetTicks() / 100, glm::vec3(0.0f, 0.0f, 1.0f));
	transform = glm::scale(transform, glm::vec3(0.5f));

	// Space / Coordinates

	Shader::Use(g_texturedShader);
	Shader::SetMat4(g_texturedShader, "transform", transform);

	g_flag->Draw();

	SDL_GL_SwapWindow(window);
}

Renderer::~Renderer() {
	if (window) {
		SDL_DestroyWindow(window);
	}
	if (context) {
		SDL_GL_DeleteContext(context);
	}
	SDL_Quit();

	std::printf("Renderer destructor done\n");
}
} // namespace FG24
