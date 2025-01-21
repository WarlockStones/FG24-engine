#pragma once
#include <thread>
#include "framework/Camera.hpp"
#include <glm/glm.hpp> // For camera initialization

namespace FG24 {
class Renderer;
class KeyInput;
class ExampleManager;
class GameState;
class EntityManager;

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
	KeyInput* keyInput{};
	GameState* m_gameState;
	ExampleManager* exampleManager;
	EntityManager* entityManager;
};
}
