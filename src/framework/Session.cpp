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

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"

namespace FG24 {
bool Session::Init() {
	renderer = new Renderer();

	if (renderer->Init() == false) {
		std::fprintf(stderr, "Error: Session failed to initialize Renderer!\n");
		return 1;
	}

	// 0. Imgui - Init imgui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	if (ImGui_ImplSDL2_InitForOpenGL(renderer->window, renderer->context) == false) {
		std::fprintf(stderr, "Error: Failed to init imgui for SDL2!\n");
		return 1;
	}
	if (ImGui_ImplOpenGL3_Init("#version 330") == false) {
		std::fprintf(stderr, "Error: Failed to init imgui for OpengL3!\n");
		return 1;
	}

	return 0;
}

void Session::Start() {
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
	// 2. Imgui - Start of update, call imgui NewFrame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	// 3. Imgui - Render the windows, buttons, or maybe just the demo window
	// ImGui::Begin("Hello");
	// ImGui::Button("Test", { 200, 200 });
	// ImGui::End();
	ImGui::ShowDemoWindow(); 
}

void Session::GameLoop() {
	constexpr int fps = 30;
	constexpr int millisecondsPerFrame = 1000 / fps;
	static double millisecondsPreviousFrame{};
	g_runGameLoop = true;
	while (g_runGameLoop) {
		KeyInput::ProcessInput(); // 1. Imgui - Send input context to imgui

		// Block game loop if running too fast
		std::uint32_t timeToWait = millisecondsPerFrame - 
			static_cast<std::uint32_t>(SDL_GetTicks() - millisecondsPreviousFrame);
		if (timeToWait > 0 && timeToWait <= millisecondsPerFrame) {
			SDL_Delay(timeToWait);
		}

		Update();
		renderer->Draw();  // 4. Imgui - Render imgui
	}
}

Session::~Session() {
	// 5. Free imgui memory
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	delete renderer;
	std::printf("Session destructor done\n");
}

} // namespace FG24
