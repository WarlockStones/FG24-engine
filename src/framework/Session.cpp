#include <cstdio>
#include <cstdint>
#include <SDL2/SDL.h>
#include "Session.hpp"
#include "Globals.hpp"
#include "renderer/Renderer.hpp"
#include "framework/KeyInput.hpp"

#include "utils/File.hpp"

// I should probably not do these rendering stuff here
#include "renderer/Material.hpp"
#include "renderer/Triangle.hpp"
#include "renderer/Shader.hpp"
#include "framework/Actor.hpp"

namespace FG24 {
bool Session::Init() {
	renderer = new Renderer();

	if (renderer->Init() == false) {
		std::fprintf(stderr, "Error: Session failed to initialize Renderer!\n");
		return 1;
	}

	return 0;
}

void Session::Start() {

	// Or should I do some of this in the renderer?
	// Factory pattern or something?
    g_simpleShader = new Shader();
	g_simpleShader
		->CompileShader("../assets/shaders/simple.vert", "../assets/shaders/simple.frag");
	std::printf("In session start shaderPogram: %u\n", g_simpleShader->program);
	g_simpleMaterial = new Material(g_simpleShader);
	g_simpleMaterial->GetShader();
	g_triangleMesh = new Triangle();
	g_triangle = new Actor(g_triangleMesh, g_simpleMaterial);
}

void Session::Update() {
}

void Session::GameLoop() {
	constexpr int fps = 30;
	constexpr int millisecondsPerFrame = 1000 / fps;
	static double millisecondsPreviousFrame{};
	g_runGameLoop = true;
	while (g_runGameLoop) {
		// TODO: Get new input
		KeyInput::ProcessInput();

		// Block game loop if running too fast
		std::uint32_t timeToWait = millisecondsPerFrame - 
			static_cast<std::uint32_t>(SDL_GetTicks() - millisecondsPreviousFrame);
		if (timeToWait > 0 && timeToWait <= millisecondsPerFrame) {
			SDL_Delay(timeToWait);
		}

		Update();
		renderer->Draw();
	}
}

Session::~Session() {
	delete renderer;
	std::printf("Session destructor done\n");
}

} // namespace FG24
