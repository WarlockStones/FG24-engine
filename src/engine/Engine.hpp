#pragma once
namespace FG24 {
	class Renderer;
	class InputManager;
class Engine {
public:
	Engine() = default;
	~Engine();
	bool Init();
	void GameLoop();

private:
	InputManager* inputManager;
	Renderer* renderer;
};
}