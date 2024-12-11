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
	// transform = glm::rotate(transform, (float)SDL_GetTicks() / 100, glm::vec3(0.0f, 0.0f, 1.0f));

	// Space / Coordinate systems
	// Local (aka. object) space > World space > View (aka. eye) space > Clip space > Screen space.
	//                >>Model Matrix>>	>>VIEW MATRIX>>>	>> PROJECTION MATRIX >> 

	// Model matrix = local to world space
	glm::mat4 model = glm::mat4(1.0f); // Identity matrix
	model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	// View matrix (camera) Move the entire scene around inversed to where we want camera to move
	// OpenGL is right-handed system so positive x is right, positive y is up. positive z is backwards.
	// So translate scene towards negative z-axis
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f)); // Move "camera"/view back -3f

	// Projection matrix. Perspective or Orthographic
	// TODO: Move perspective function call away from update loop, it only needs to run once
	glm::mat4 proj = glm::perspective(
		glm::radians(45.0f), // FOV
		static_cast<float>(g_windowWidth) / static_cast<float>(g_windowHeight), // Aspect ratio
		0.1f, 100.0f); // near, far planes of the frustrum

	Shader::Use(g_texturedShader);
	Shader::SetMat4(g_texturedShader, "model", model);
	Shader::SetMat4(g_texturedShader, "view", view);
	Shader::SetMat4(g_texturedShader, "projection", proj);

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
