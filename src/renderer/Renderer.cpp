// Copyright 2024 Changemaker Educations AB. All Rights Reserved
#include "Renderer.hpp"
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL.h>
#include <cstdio>
#include "../engine/core/actor/Actor.hpp"
#include "mesh/Mesh.hpp"
#include "camera/Camera.hpp"
#include "shader/Shader.hpp"

#include "../engine/inputmanager/InputManager.hpp" // Temproary solution

namespace FG24 {
bool Renderer::Init(int windowWidth, int windowHeight) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::fprintf(stderr, "Error initializing SDL!\n");
		return 1;
	}

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	uint32_t windowFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_MOUSE_GRABBED;

	window = SDL_CreateWindow("FG24 Engine", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, windowFlags);

	context = SDL_GL_CreateContext(window);

	if (!gladLoadGL(GLADloadfunc(SDL_GL_GetProcAddress))) {
		std::fprintf(stderr, "Error: Failed to initialize GLAD!\n");
		return 1;
	}

	// SDL Settings
	SDL_SetRelativeMouseMode(SDL_TRUE);

	// 0 = none, 1 = vsync, -1 = adaptive vsync
	if (SDL_GL_SetSwapInterval(0) != 0) {
		std::fprintf(stderr, "Error: Setting SDL swap interval!\n");
		std::fprintf(stderr, "%s\n", SDL_GetError());
		return 1;
	}

	// Temporary. Move camera somewhere else later
  camera = Camera(glm::vec3(-10.0f, 0.0f, 0.0f),  // Position
			glm::vec3(1.0f, 0.0f, 1.0f), // Front
			glm::vec3(0.0f, 1.0f, 0.0f), // Up
			45, 20.0f, 0.1f); // Fov, Speed, Mouse sensitivity
	
	
	return 0;
}

  void Renderer::Update(Actor** actors, int actorCount) {
		glClearColor(0.21f, 0.21f, 0.21f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// TODO: Remove projection matrix from update loop
		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(90.0f),
									  static_cast<float>(800) /
									  static_cast<float>(600),
									  0.1f,
									  100.0f);

		// TODO: set all the transforms to the shader
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 model = glm::mat4(1.0f);
		
		
		// All boxes share the same shader atm
		Shader& s = *actors[0]->shader;
		s.Use(); // Must use before setting shit
		// s.SetMat4("projection", projection);
		// s.SetMat4("view", view);
		s.SetMat4("model", model);

		// std::printf("Camera: %f.%f.%f\n", camera.position.x, camera.position.y, camera.position.z);
		const glm::vec3& p = actors[0]->transform.position;
		const auto cameraPos = camera.position;
		const auto cameraDir = camera.front;
		const auto cameraUp = camera.up;
		// std::printf("Actor 1: %f.%f.%f\n", p.x, p.y, p.z);
		
		for (int i = 0; i < actorCount; ++i)
		  {
			// Hmm... This code. This structure... ???
			actors[i]->mesh->Render(s);
		  }
		
		SDL_GL_SwapWindow(window);
	}

	Renderer::~Renderer() {
		if (window) {
			SDL_DestroyWindow(window);
			window = nullptr;
		}
		SDL_GL_DeleteContext(context);
		SDL_Quit();

		std::printf("Renderer destructor done\n");
	}
} // namespace FG24
