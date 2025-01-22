#include "Renderer.hpp"
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SDL2/SDL.h>
#include <vector>
#include <cstdio>
#include "Globals.hpp"
#include "framework/Entity.hpp"
#include "Shader.hpp"
#include "framework/Camera.hpp"
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

	m_window = SDL_CreateWindow("FG24 Engine", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, g_windowWidth, g_windowHeight, windowFlags);

	m_context = SDL_GL_CreateContext(m_window);

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

	glEnable(GL_DEPTH_TEST);

	SetProjectionMatrix(45.0f, g_windowWidth, g_windowHeight);

	return true;
}

void Renderer::Draw(const std::vector<Entity*>& entities) const {
	glClearColor(0.21f, 0.21f, 0.21f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Space / Coordinate systems
	// Local/object space > World space > View/eye space > Clip space > Screen space.

	// TODO: Support more than one shaderID
	Shader::Use(g_shader);
	Shader::SetMat4(g_shader, "view", g_camera->GetViewMatrix());
	Shader::SetMat4(g_shader, "projection", m_projection);
	Shader::SetVec3(g_shader, "cameraPosition", g_camera->GetPosition());

	for(const Entity* e : entities) {
		glm::mat4 tr = glm::mat4(1);
		glm::mat4 rot = glm::mat4(1);
		glm::mat4 scl = glm::mat4(1);
		tr = glm::translate(tr, e->m_transform.GetLocation());
		rot = e->m_transform.GetRotationMatrix();
		scl = glm::scale(scl, e->m_transform.GetScale());
		glm::mat4 model = tr * rot * scl;
		Shader::SetMat4(g_shader, "model", model);
		e->Draw();
	}

	// Light
	// Shader::SetVec3(g_shader, "lightPosition", g_light->m_transform.location);
	/*
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, g_light->m_transform.location); // Translate first!
	lightModel = glm::scale(lightModel, glm::vec3(0.5));
	// lightModel = glm::rotate(lightModel, ???));
	Shader::SetMat4(g_shader, "model", lightModel);
	g_light->Draw();

	// Light 2
	glm::mat4 lm2 = glm::mat4(1.0);
	lm2 = glm::translate(lm2, g_light2->m_transform.location); // Translate first!
	lm2 = glm::scale(lm2, glm::vec3(0.5));
	Shader::SetMat4(g_shader, "model", lm2);
	g_light2->Draw();
	*/
	
	SDL_GL_SwapWindow(m_window);
}

void Renderer::SetProjectionMatrix(float fov, std::uint32_t windowWidth, std::uint32_t windowHeight, float nearClipPlane, float farClipPlane) {
	// Don't run perspective calculation in update loop.
	// Projection matrix only needs to calculate on FOV, aspect ratio, or clip-plane changes
	m_projection = glm::perspective(
		glm::radians(fov),
		static_cast<float>(g_windowWidth) / static_cast<float>(g_windowHeight),
		0.1f, 100.0f);

	// Can be changed to orthographic view if such camera is desired
}

Renderer::~Renderer() {
	if (m_window) {
		SDL_DestroyWindow(m_window);
	}
	if (m_context) {
		SDL_GL_DeleteContext(m_context);
	}
	SDL_Quit();

	std::printf("Renderer destructor done\n");
}
} // namespace FG24
