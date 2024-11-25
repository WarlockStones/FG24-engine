#pragma once
#include <cstdint>

namespace FG24 {
class Material;
class Mesh;

// Actor is a thing in the world that has graphics and can be drawn
class Actor {
public:
	Actor(Mesh* mesh, std::uint32_t shader, std::uint32_t texture = 0);
	~Actor() = default;
	void Draw();

private:
	Mesh* mesh{};
	std::uint32_t shader{};
	std::uint32_t texture = 0; // Texture id 0 means has no texture
};
} // namespace FG24
