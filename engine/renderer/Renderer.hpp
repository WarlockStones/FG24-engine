#pragma once
#include <vector>
#include <glm/mat4x4.hpp>
#include "framework/Entity.hpp"
struct SDL_Window;

namespace FG24 {
class Renderer {
public:
	~Renderer();
	bool Init();
	void Draw(const std::vector<Entity*>& entities) const;
	void SetProjectionMatrix(float fov,
		std::uint32_t windowWidth,
		std::uint32_t windowHeight,
		float nearClipPlane = 0.1f,
		float farClipPlane = 100.0f);

	SDL_Window* m_window{};
	void* m_context{}; // SDL_GLContext is a void* typedef // TODO: Cast to something sensible
private:
	// Translation matrices
	glm::mat4 m_view;
	glm::mat4 m_projection;

};
}
