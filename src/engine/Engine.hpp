#pragma once
namespace FG24 {
class Engine {
public:
	Engine() = default;
	~Engine();
	bool Init();
	void GameLoop();

	class Renderer* renderer;
};
}