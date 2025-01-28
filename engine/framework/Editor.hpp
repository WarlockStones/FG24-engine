#pragma once
#include "renderer/Renderer.hpp"
#include "framework/EntityManager.hpp"

namespace FG24 {
namespace Editor {
	void Init(const Renderer& renderer);
	void Draw(EntityManager& entityManager);
    void Destroy();
} // namespace Editor
} // namespace FG24
