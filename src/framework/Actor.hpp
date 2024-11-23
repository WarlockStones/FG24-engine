#pragma once

namespace FG24 {
class Material;
class Mesh;

// Actor is a thing in the world that has graphics and can be drawn
class Actor {
public:
	Actor(Mesh* mesh, Material* material);
	~Actor() = default;

	void Draw();

private:
	Mesh* mesh{};
	Material* material{};
};
} // namespace FG24
