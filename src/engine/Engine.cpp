#include "Engine.hpp"
#include "../renderer/Renderer.hpp"
#include <stdio.h>

namespace FG24 {
	bool Engine::Init() {
		renderer = new Renderer();
		if (renderer->Init(800, 600) != 0) {
			fprintf(stderr, "Error: Engine failed to initialize Renderer!\n");
			return 1;
		}

		return 0;
	}

	void Engine::GameLoop()
	{
		while (true)
		{
			renderer->Update();
		}
	}

	Engine::~Engine()
	{
		delete renderer;
		printf("Engine destructor done\n");
	}
}
