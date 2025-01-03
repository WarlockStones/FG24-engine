#pragma once
#include <thread>

namespace FG24 {
class Renderer;
class KeyInput;
class ExampleManager;

class Session {
public:
	Session() = default;
	bool Init();
	void Start();
	void Update();
	void GameLoop();
	~Session();
private:
	Renderer* renderer{};
	KeyInput* keyInput{};

	ExampleManager* exampleManager;
};
}
