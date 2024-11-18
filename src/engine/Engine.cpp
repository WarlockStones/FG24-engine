// Copyright 2024 Changemaker Educations AB. All Rights Reserved
#include "Engine.hpp"
#include "../renderer/Renderer.hpp"
#include "inputmanager/InputManager.hpp"
#include <cstdio>
#include "../renderer/mesh/shapes/Cube.hpp"
#include "../renderer/shader/Shader.hpp"
#include "core/actor/Actor.hpp"
#include <cassert>
#include "../Globals.hpp"

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
static constexpr int boxCount = 24;
static Actor* boxes[boxCount] {nullptr};

void Engine::Start() {
	Cube* cube = new Cube();

	// Shader* shader = new Shader("../assets/shaders/shader.vert", "../assets/shaders/shader.frag");
	Shader* shader = new Shader("..\\assets\\shaders\\shader.vert", "..\\assets\\shaders\\shader.frag");
	assert(shader);
	
	for (int i = 0; i < boxCount; ++i)
	{
		static int yOffset = 0;
		boxes[i] = new Actor(cube, shader, Transform());
		boxes[i]->transform.position.z += 5;
	}

	assert(boxes[4]);
	
}

void Engine::Update() {
}

void Engine::GameLoop()	{
	const int FPS {14};
	const int MILLISECS_PER_FRAME = {1000 / FPS} ;
	static double millisecsPreviousFrame{}; // 0. First frame = current frame
	g_isRunning = true;
	while (g_isRunning) {
		renderer->camera.ProcessInput();

		int timeToWait =
		MILLISECS_PER_FRAME - (SDL_GetTicks() - millisecsPreviousFrame);
		// Protection. If we HAVE to wait and we are too fast. Delay.
		if (timeToWait > 0 && timeToWait <= MILLISECS_PER_FRAME) {
			SDL_Delay(timeToWait);
		}
		
		g_deltaTime = (SDL_GetTicks() - millisecsPreviousFrame) / 1000.0;
		millisecsPreviousFrame = SDL_GetTicks(); // Store frame-time

		Update();
		renderer->Update(boxes, boxCount);
	}
}


Engine::~Engine() {
	std::printf("Engine destructor started\n");
	delete renderer;
	delete inputManager;
	std::printf("Engine destructor done\n");
}
}
