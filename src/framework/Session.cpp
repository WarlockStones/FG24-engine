#include <cstdio>
#include <cstdint>
#include <cassert>
#include <SDL2/SDL.h>
#include "Session.hpp"
#include "Globals.hpp"
#include "renderer/Renderer.hpp"
#include "framework/KeyInput.hpp"

#include "utils/File.hpp"

// I should probably not do these rendering stuff here
#include "renderer/Triangle.hpp"
#include "renderer/Shader.hpp"
#include "renderer/Texture.hpp"
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
    // g_simpleShader = new Shader();
	// g_simpleShader
	// ->CompileShader("../assets/shaders/simple.vert", "../assets/shaders/simple.frag");
	//	std::printf("In session start shaderPogram: %u\n", g_simpleShader->program);
	// g_arcadeTexture->LoadFromFile("../assets/textures/arcade_carpet.png");
	// g_simpleMaterial = new Material(g_simpleShader, g_arcadeTexture);
	// g_simpleMaterial->GetShader();
	// g_triangleMesh = new Triangle();
	// g_triangle = new Actor(g_triangleMesh, g_simpleMaterial);

	// Actor needs mesh, uint32 shaderId, uint32 textureId
    // 1. Load shader from file.
    // 2. Load texture from file.
    // 3. mesh for square
    // 4. Load all of that into a new actor
	// g_simpleShader = Shader::CompileShader("../assets/shaders/simple.vert",
	//									 "../assets/shaders/simple.frag");

	// TODO: change name of shader.vert to be something to do with textures.
    // TODO: change name of Triangle.cpp to be SimpleShapes.cpp

    g_texturedShader = Shader::CompileShader("../assets/shaders/textured.vert",
										   "../assets/shaders/textured.frag");
	assert(g_texturedShader != 0);
	g_arcadeTexture = Texture::LoadFromFile("../assets/textures/arcade_carpet.png");
	assert(g_arcadeTexture != 0);
	g_triangleMesh = new Square();
	g_triangle = new Actor(g_triangleMesh, g_texturedShader, g_arcadeTexture);
	
	assert(g_triangle);
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
