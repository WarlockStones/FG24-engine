#include "Renderer.hpp"
#include <glad/gl.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL.h>
#include <stdio.h>

namespace FG24 {
	bool Renderer::Init(int WindowWidth, int WindowHeight) {
	  if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	  {
		fprintf(stderr, "Error initializing SDL!\n");
		return 1;
	  }

	  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	  uint32_t uWindowFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_MOUSE_GRABBED;
	  pWindow = SDL_CreateWindow("FG24 Engine", SDL_WINDOWPOS_CENTERED,
		  SDL_WINDOWPOS_CENTERED, WindowWidth, WindowHeight, uWindowFlags);

	  Context = SDL_GL_CreateContext(pWindow);

	  if (!gladLoadGL(GLADloadfunc(SDL_GL_GetProcAddress))) {
		  fprintf(stderr, "Error: Failed to initialize GLAD!\n");
		  return 1;
	  }

	  // SDL Settings
	  SDL_SetRelativeMouseMode(SDL_TRUE);

	  // 0 = none, 1 = vsync, -1 = adaptive vsync
	  if (SDL_GL_SetSwapInterval(0) != 0)
	  {
		  fprintf(stderr, "Error: Setting SDL swap interval!\n");
		  fprintf(stderr, "%s\n", SDL_GetError());
		  return 1;
	  }
	  
	  return 0;
	}

	void Renderer::Update()
	{
		glClearColor(0.21f, 0.21f, 0.21f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		SDL_GL_SwapWindow(pWindow);
	}

	Renderer::~Renderer()
	{
		if (pWindow)
		{
			SDL_DestroyWindow(pWindow);
			pWindow = nullptr;
		}
		SDL_GL_DeleteContext(Context);
		SDL_Quit();

		printf("Renderer destructor done\n");
	}
}