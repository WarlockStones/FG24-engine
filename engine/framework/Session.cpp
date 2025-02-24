#include <cstdio>
#include <cstdint>
#include <cassert>
#include <SDL2/SDL.h>
#include "Session.hpp"
#include "Globals.hpp"
#include "renderer/Renderer.hpp"
#include "framework/KeyInput.hpp"
#include "framework/CameraManager.hpp"
#include "framework/Editor.hpp"
#include <vector>

#include "utils/File.hpp"
#include "framework/Lighting.hpp"

// I should probably not do these rendering stuff here
#include "renderer/Shader.hpp"
#include "renderer/Texture.hpp"
#include "renderer/VertexData.hpp"
#include "framework/Entity.hpp"
#include "renderer/Mesh.hpp"
#include "framework/EntityManager.hpp"
#include "MeshManager.hpp"

// Temp testing
#include "framework/ExampleManager.hpp"
#include "framework/Message.hpp"

#include "physics/PhysicsSimulation.hpp"
#include "physics/Collider.hpp"
#include "physics/SphereCollider.hpp"
#include "physics/BoxCollider.hpp"


namespace FG24 {
bool Session::Init() {
	renderer = new Renderer();

	if (renderer->Init() == false) {
		std::fprintf(stderr, "Error: Session failed to initialize Renderer!\n");
		return 1;
	}

	Editor::Init(*renderer);

	return 0;
}


void Session::Start() {
	// ----- Load assets -----
	g_shader = Shader::CompileShader("../../assets/shaders/phongshadow.vert",
									 "../../assets/shaders/phongshadow.frag");
	assert(g_shader != 0);
	g_blendShader = Shader::CompileShader("../../assets/shaders/phongshadowblend.vert",
										  "../../assets/shaders/phongshadow.frag");
	assert(g_blendShader != 0);
	g_flatShader = Shader::CompileShader("../../assets/shaders/flat.vert",
										 "../../assets/shaders/flat.frag");
	assert(g_flatShader != 0);
	g_flatBlendShader = Shader::CompileShader("../../assets/shaders/flatblend.vert",
											  "../../assets/shaders/flat.frag");
	assert(g_flatBlendShader != 0);

	std::uint32_t arcadeTex(Texture::LoadFromFile("../../assets/textures/arcade_carpet.png", "Arcade"));
	assert(arcadeTex != 0);
	std::uint32_t helloTex(Texture::LoadFromFile("../../assets/textures/hello.png", "Hello"));

	Texture::LoadFromFile("../../assets/textures/barrel_albedo.png", "Barrel_albedo");
	Texture::LoadFromFile("../../assets/textures/barrel_specular.png", "Barrel_specular");
	
	Texture::LoadFromFile("../../assets/textures/barrel2.png", "Barrel2_albedo");
	Texture::LoadFromFile("../../assets/textures/barrel2_specular.png", "Barrel2_specular");

	Texture::LoadFromFile("../../assets/textures/box_uv_albedo.png", "Box_uv_albedo");
	Texture::LoadFromFile("../../assets/textures/uv_test.png", "uv_test");

	// Load mesh
	// TODO: Don't assert, use default mesh
	VertexData monkeyData = MeshManager::LoadVertexData("suzanne_tri");
	Mesh* monkeyMesh = MeshManager::AddMesh("suzanne_tri", monkeyData);
	VertexData cubeData = MeshManager::LoadVertexData("cube");
	g_cubeMesh = MeshManager::AddMesh("cube", cubeData);
	auto sphereData = MeshManager::LoadVertexData("sphere");
	g_sphereMesh = MeshManager::AddMesh("sphere", sphereData);
	auto barrel = MeshManager::LoadVertexData("barrel");
	MeshManager::AddMesh("barrel", barrel);
	auto barrel2 = MeshManager::LoadVertexData("barrel2");
	MeshManager::AddMesh("barrel2", barrel2);
	auto boxUv = MeshManager::LoadVertexData("box_uv");
	MeshManager::AddMesh("box_uv", boxUv);
	auto blend1 = MeshManager::LoadVertexData("blend1");
	auto blend2 = MeshManager::LoadVertexData("blend2");
	MeshManager::AddMesh("blend", blend1, blend2);
	auto pieData = MeshManager::LoadVertexData("triangtest_pie");
	MeshManager::AddMesh("triangtest_pie", pieData);

	// ----- Configure entities -----
	g_entity1 = entityManager.CreateEntity(monkeyMesh, g_shader, "Monkey");

	g_entity2 = entityManager.CreateEntity(g_cubeMesh, g_shader, "Box");
	g_entity2->m_transform.SetLocation(glm::vec3(2, 2, -2));
	g_entity2->m_transform.SetScale(glm::vec3(0.2f, 0.2f, 0.2f));

	auto wall = entityManager.CreateEntity(g_cubeMesh, g_shader, "Wall");
	wall->m_transform.SetLocation(glm::vec3(0, 0, -3.5));
	wall->m_transform.SetScale(glm::vec3(4.0f, 4.0f, 1.0f));

	// Lights
	g_light1 = Lighting::CreateLight(
		glm::vec3(2, 2, 4), // Pos
		LightType::Point,    // Type
		glm::vec4(0.0f, 0.7f, 0.0f, 1.0f), // diffuse
		glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), // specular
		glm::vec3(1.0f, 0.005f, 0.0005f)
		);

	g_light2 = Lighting::CreateLight(
		glm::vec3(-2, 0, -2),
		LightType::Point,
		glm::vec4(0.7f, 0.7f, 0.7f, 1.0f),
		glm::vec4(0.7f, 0.7f, 0.7f, 1.0f));

	g_light3 = Lighting::CreateLight(
		glm::vec3(1, 0, 1),
		LightType::Spot,
		glm::vec4(0.5f, 0.0f, 0.5f, 1.0f),
		glm::vec4(0.5f, 0.0f, 0.5f, 1.0f),
		glm::vec3(2.0f, 0.05f, 0.005), // Attenuation
		glm::vec3(0, 0, -1)); // rotation

	g_lightDir = Lighting::CreateLight(
		glm::vec3(4, 5, 10), // Fit to ShadowMap Light POV Camer.
		LightType::Directional,
		glm::vec4(0.7f, 0.6f, 0.5f, 1.0f),
		glm::vec4(0.7f, 0.6f, 0.5f, 1.0f));

	// exampleManager->StartThread();
	
	auto startCamId = CameraManager::CreateCamera(glm::vec3(0,0,4), -90, 0);
	CameraManager::SetActiveCamera(startCamId);
	CameraManager::CreateCamera(glm::vec3(-3,2,2), -45, -45);

	// Create physics collider objects
	auto* s1 = new BoxCollider();
	// s1->m_radius = 1;
	s1->m_transform.SetLocation(glm::vec3(0, 0, 0));
	s1->m_hasGravity = false;
	// s1->m_extents = 1;
	PhysicsSimulation::AddCollider(s1);


	auto* s2 = new SphereCollider();
	s2->m_radius = 1;
	s2->m_transform.SetLocation(glm::vec3(0, 15, 0));
	PhysicsSimulation::AddCollider(s2);

	auto* b1 = new BoxCollider();
	b1->m_transform.SetLocation(glm::vec3(0.5f, 5, 0.5f));
	PhysicsSimulation::AddCollider(b1);

	
	return;

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
		Editor::Draw(entityManager);
		renderer->DrawLightOnly(entityManager.GetEntities());
		renderer->Draw(entityManager.GetEntities(), PhysicsSimulation::GetColliders());
	}
}

static bool lightShouldTick = true;
// Update game state
void Session::Update(float deltaTime) {
	{
		// Update physics
		using namespace PhysicsSimulation;
		ApplyGravity(deltaTime);
		HandleCollisions(CheckIntersections());
		ApplyVelocity(deltaTime);
	}

	// Testing sending messages to manager on another thread
	if (g_action1) { // Press keyboard key 1
		std::uint32_t ms = SDL_GetTicks();
		float s = static_cast<float>(ms) / 1000.0f;
		printf("Session is sending a message\n");
		exampleManager.QueueMessage(new FloatMessage(s));
		g_action1 = false;
	}

	auto* c = CameraManager::GetActiveCamera();
	if (c) {
		c->Update(deltaTime);
	}

	// float rot = static_cast<float>(SDL_GetTicks()) * 0.001f;
	// g_entity2->m_transform.SetRotation(glm::vec3(0, rot, 0.0f));

	if (g_action2) {
		g_action2 = false;
		lightShouldTick = !lightShouldTick;
	}
	auto& lights = Lighting::GetLights();
	if (lightShouldTick) {
		static float lightOffset = 0;
		lightOffset += deltaTime;
		if (lights.size() > 0) {
			lights[0]->m_position = glm::vec3(
				glm::sin(lightOffset * 1.1f) * 4.0f - 1,
				3,
				glm::cos(lightOffset * 1.1f) * 4.0f - 1);
		}
	}
	if (lights.size() > 1) {
		static float pos = 0;
		pos += deltaTime;
		lights[1]->m_position = glm::vec3(
			lights[1]->m_position.x,
			lights[1]->m_position.y,
			glm::sin(pos * 1.1) * 4.0f + 4.5f);
	}

	auto hit = PhysicsSimulation::Raycast(g_rayOrigin, g_rayDir);
	// using DebugRay = std::tuple<glm::vec3, glm::vec3, float>;
	// static std::vector<DebugRay> debugRays;
	if (hit.has_value()) {
		if (hit.value().m_collider->m_type == ColliderType::Box) {
			std::printf("Ray Box Intersection\n");
		} else {
			std::printf("Ray Sphere Intersection\n");
		}
	}
}

Session::~Session() {
	if (renderer != nullptr) {
		delete renderer;
	}
	Editor::Destroy();
	std::printf("Session destructor done\n");
}

} // namespace FG24
