// Copyright 2024 Changemaker Educations AB. All Rights Reserved
#include "Engine.hpp"
#include "../renderer/Renderer.hpp"
#include "inputmanager/InputManager.hpp"
#include <cstdio>
#include "../renderer/mesh/shapes/Cube.hpp"
#include "../renderer/shader/Shader.hpp"
#include "core/actor/Actor.hpp"
#include <cassert>

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

// Temporary debug thing until I have a proper structure of game object management
constexpr int boxCount = 24;
Actor* boxes[boxCount] {nullptr};

void Engine::Start() {
	Cube* cube = new Cube();

	Shader* shader = new Shader("../assets/shaders/shader.vert", "../assets/shaders/shader.frag");
	assert(shader);
	
	for (int i = 0; i < boxCount; ++i)
	{
		static int yOffset = 0;
		boxes[i] = new Actor(cube, shader, Transform());
		boxes[i]->transform.position.y += 100;
	}

	assert(boxes[4]);
	
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
