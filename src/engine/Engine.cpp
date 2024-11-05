#include "Engine.hpp"
#include "../renderer/Renderer.hpp"
#include "inputmanager/InputManager.hpp"
#include <stdio.h>

namespace FG24 {
	bool Engine::Init() {
		renderer = new Renderer();
		if (renderer->Init(800, 600) != 0) {
			fprintf(stderr, "Error: Engine failed to initialize Renderer!\n");
			return 1;
		}

		inputManager = new InputManager();
		return 0;
	}

	void Engine::GameLoop()	{
		while (inputManager->ProcessInputs()) {
			renderer->Update();
		}
	}


	Engine::~Engine() {
		printf("Engine destructor started\n");
		delete renderer;
		delete inputManager;
		printf("Engine destructor done\n");
	}
}
