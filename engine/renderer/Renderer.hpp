#pragma once
#include <vector>
#include "framework/Entity.hpp"
struct SDL_Window;

namespace FG24 {
class Renderer {
public:
	~Renderer();
	bool Init();
	void Draw(const std::vector<Entity>& entities) const;

private:
	SDL_Window* window{};
	void* context{}; // SDL_GLContext is a void* typedef // TODO: Cast to something sensible
};
}