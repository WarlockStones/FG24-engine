#pragma once
#include "renderer/Renderer.hpp"

namespace FG24 {
namespace Editor {
	bool Init(const Renderer& renderer);
	void Draw();
    void Destroy();
} // namespace Editor
} // namespace FG24
