// Copyright 2024 Changemaker Educations AB. All Rights Reserved
#include "Renderer.hpp"
#include <glad/gl.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL.h>
#include <cstdio>

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

	return 0;
	}

	void Renderer::Update() {
		glClearColor(0.21f, 0.21f, 0.21f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
}