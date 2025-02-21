#pragma once
#include <thread>
#include <vector>
#include "framework/Camera.hpp"
#include <glm/glm.hpp> // For camera initialization
#include "framework/KeyInput.hpp"
#include "framework/ExampleManager.hpp"
#include "framework/EntityManager.hpp"

namespace FG24 {
class Renderer;
class KeyInput;
class ExampleManager;
class EntityManager;
class Collider;

class Session {
public:
	Session() = default;
	bool Init();
	void Start();
	void Update(float deltaTime);
	void GameLoop();
	~Session();
private:
	Renderer* renderer{};
	KeyInput keyInput;
	ExampleManager exampleManager;
	EntityManager entityManager;
};
}
