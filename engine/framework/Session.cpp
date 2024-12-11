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
#include "renderer/SimpleShapes.hpp"
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
	// Or should I do some of this in the renderer? Factory pattern or something?

	g_texturedShader = Shader::CompileShader("../../assets/shaders/textured.vert",
											 "../../assets/shaders/textured.frag");
	assert(g_texturedShader != 0);
	g_arcadeTexture = Texture::LoadFromFile("../../assets/textures/arcade_carpet.png");
	assert(g_arcadeTexture != 0);
	g_triangleMesh = new Square();
	g_triangle = new Actor(g_triangleMesh, g_texturedShader, g_arcadeTexture);
	assert(g_triangle);
	MeshData flagData = FG24::File::LoadObjToMeshData("../../assets/mesh/cube.obj");
	Mesh* flagMesh = new Mesh(flagData);
	Cube* cube = new Cube();
	g_flag = new Actor(cube, g_texturedShader, g_arcadeTexture);

#ifdef false
	// Testing serialization
	// If save file for this actor exists, load it.
	Vec3 v;
	v.x = 7000;
	v.y = 10;
	v.z = 10;
	g_flag->transform.location = v;

	FILE* fp = std::fopen("../../assets/flag.sav", "rb"); 
	if (fp) { // Fails if file does not exist since it uses "r" - read mode
		std::printf("Reading...");
		g_flag->ReadFrom(fp);
	} else {
		fp = std::fopen("../../assets/flag.sav", "wb"); // Open for writing
		if (fp) {
			std::printf("Writing...\n");
			g_flag->WriteTo(fp);
		} else {
			std::printf("Error: failed to open file for flag asset save\n");
		}
	}

	std::printf("Vec3 size = %lu\n", sizeof(Vec3));
	std::printf("float size = %lu\n", sizeof(float));
	const Vec3& loc = g_flag->transform.location;
	const Vec3& rot= g_flag->transform.rotation;
	const Vec3& scl= g_flag->transform.scale;
	std::printf("loc: %f %f %f\n", loc.x, loc.y, loc.z);
	std::printf("rot: %f %f %f\n", rot.x, rot.y, rot.z);
	std::printf("scl: %f %f %f\n", scl.x, scl.y, scl.z);

	if (fp) std::fclose(fp);
#endif
}

void Session::Update() {
}

void Session::GameLoop() {
	constexpr int fps = 30;
	constexpr int millisecondsPerFrame = 1000 / fps;
	static double millisecondsPreviousFrame{};
	g_runGameLoop = true;
	while (g_runGameLoop) {
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
