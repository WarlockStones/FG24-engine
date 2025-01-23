#include <cstdio>
#include <cstdint>
#include <cassert>
#include <SDL2/SDL.h>
#include "Session.hpp"
#include "Globals.hpp"
#include "renderer/Renderer.hpp"
#include "framework/KeyInput.hpp"

#include "utils/File.hpp"
#include "framework/Lighting.hpp"

// I should probably not do these rendering stuff here
#include "renderer/Shader.hpp"
#include "renderer/Texture.hpp"
#include "framework/Entity.hpp"
#include "renderer/Mesh.hpp"
#include "framework/EntityManager.hpp"
#include "framework/Lighting.hpp"

// Temp testing
#include "framework/ExampleManager.hpp"
#include "framework/Message.hpp"


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
	// ----- Load assets -----
	g_shader = Shader::CompileShader("../../assets/shaders/phong.vert",
									 "../../assets/shaders/phong.frag");
	assert(g_shader != 0);
	g_arcadeTexture = Texture::LoadFromFile("../../assets/textures/arcade_carpet.png");
	assert(g_arcadeTexture != 0);

	// Add data
	float* monkeyVertexData = nullptr;
	std::size_t numFlagVertexData{};
	std::size_t numFlagVerticies{};
	auto ec = FG24::File::LoadObjToMeshData(
		"../../assets/mesh/suzanne_tri.obj",
		monkeyVertexData,
		numFlagVertexData,
		numFlagVerticies);
	// TODO: Use default mesh if ErrorCode != Ok
	assert(ec == File::ErrorCode::Ok);

	// Add cube
	float* cubeVertexData = nullptr;
	std::size_t numCubeVertexData{};
	std::size_t numCubeVerticies{};
	ec = FG24::File::LoadObjToMeshData( // TODO: Make this a default mesh
		"../../assets/mesh/cube.obj", 
		cubeVertexData,
		numCubeVertexData,
		numCubeVerticies);
	assert(ec == File::ErrorCode::Ok);
	g_cubeMesh = new Mesh;
	g_cubeMesh->InitBuffers(cubeVertexData, numCubeVertexData, numFlagVerticies);

	// ----- Configure entities -----
	Mesh* monkeyMesh = new Mesh;
	monkeyMesh->InitBuffers(monkeyVertexData, numFlagVertexData, numFlagVerticies);
	// Monkey
	g_entity1 = entityManager.CreateEntity(*monkeyMesh, g_shader);

	// Box
	g_entity2 = entityManager.CreateEntity(*g_cubeMesh, g_shader);
	g_entity2->m_transform.SetLocation(glm::vec3(2, 2, -2));
	g_entity2->m_transform.SetScale(glm::vec3(0.2f, 0.2f, 0.2f));

	// Lights
	g_light1 = Lighting::CreateLight(
		glm::vec3(2, 2, 4), // Pos
		LightType::Point,    // Type
		glm::vec4(0.0f, 0.7f, 0.0f, 1.0f),	// diffuse
		glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)); // specular

	g_light2 = Lighting::CreateLight(
		glm::vec3(-2, 0, -2),
		LightType::Point,
		glm::vec4(0.7f, 0.7f, 0.7f, 1.0f),
		glm::vec4(0.7f, 0.7f, 0.7f, 1.0f));

	g_light3 = Lighting::CreateLight(
		glm::vec3(2, 0, 2),
		LightType::Point,
		glm::vec4(0.5f, 0.0f, 0.5f, 1.0f),
		glm::vec4(0.5f, 0.0f, 0.5f, 1.0f));

	// exampleManager->StartThread();
	
	g_camera = new Camera(glm::vec3(0, 0, 4), -90, 0);

#if false // Test serialization
	// If save file for this entity exists, load it.
	Vec3 v;
	v.x = 7000;
	v.y = 10;
	v.z = 10;
	g_flag->transform.location = v;

	FILE* fp = std::fopen("../../assets/flag.sav", "rb"); 
	if (fp) { // Fails if file does not exist since it uses "r" - read mode
		std::printf("Reading...");
		g_monkey->ReadFrom(fp);
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

void Session::GameLoop() {
	constexpr int fps = 60;
	constexpr int millisecondsPerFrame = 1000 / fps;
	static std::uint32_t millisecondsPreviousFrame{};
	g_runGameLoop = true;
	while (g_runGameLoop) {
		// Lock fps
		std::uint32_t timeToWait = millisecondsPerFrame - 
			static_cast<std::uint32_t>(SDL_GetTicks() - millisecondsPreviousFrame);
		if (timeToWait > 0 && timeToWait <= millisecondsPerFrame) {
			SDL_Delay(timeToWait);
		}
		float deltaTime = (SDL_GetTicks() - millisecondsPreviousFrame) / 1000.0;
		millisecondsPreviousFrame = SDL_GetTicks();

		KeyInput::ProcessInput();
		Update(deltaTime); 
		renderer->Draw(entityManager.GetEntities());
	}
}

static bool lightShouldTick = true;
// Update game state
void Session::Update(float deltaTime) {

	// Testing sending messages to manager on another thread
	if (g_action1) { // Press keyboard key 1
		std::uint32_t ms = SDL_GetTicks();
		float s = static_cast<float>(ms) / 1000.0f;
		printf("Session is sending a message\n");
		exampleManager.QueueMessage(new FloatMessage(s));
		g_action1 = false;
	}

	g_camera->Update(deltaTime);

	float rot = static_cast<float>(SDL_GetTicks()) * 0.1f;
	g_entity2->m_transform.SetRotation(glm::vec3(0, rot, 0.0f));

	if (g_action2) {
		g_action2 = false;
		lightShouldTick = !lightShouldTick;
	}
	if (lightShouldTick) {
		static float lightOffset = 0;
		lightOffset += deltaTime;
		g_light1->m_position = glm::vec3(
			glm::sin(lightOffset * 1.1f) * 4.0f - 1,
			3,
			glm::cos(lightOffset * 1.1f) * 4.0f - 1);
	}
}

Session::~Session() {
	if (renderer != nullptr) {
		delete renderer;
	}
	std::printf("Session destructor done\n");
}

} // namespace FG24
