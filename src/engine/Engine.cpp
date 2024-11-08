// Copyright 2024 Changemaker Educations AB. All Rights Reserved
#include "Engine.hpp"
#include "../renderer/Renderer.hpp"
#include "inputmanager/InputManager.hpp"
#include <cstdio>

namespace FG24 {
bool Engine::Init() {
	renderer = new Renderer();

	if (renderer->Init(800, 600) != 0) {
		std::fprintf(stderr, "Error: Engine failed to initialize Renderer!\n");
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
	std::printf("Engine destructor started\n");
	delete renderer;
	delete inputManager;
	std::printf("Engine destructor done\n");
}
}
