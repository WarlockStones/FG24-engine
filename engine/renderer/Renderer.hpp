#pragma once

struct SDL_Window;

namespace FG24 {
class Renderer {
public:
	bool Init();
	void Draw();
	~Renderer();

private:
	SDL_Window* window{};
	void* context{}; // SDL_GLContext is a void* typedef
};
}