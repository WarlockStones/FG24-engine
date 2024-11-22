#pragma once

namespace FG24 {
class Material;
class Mesh;

// Actor is a thing in the world that has graphics and can be drawn
class Actor {
public:
	Actor(Material* material, Mesh* mesh);
	~Actor() = default;

	void Draw();

private:
	Material* material;
	Mesh* mesh;
};
} // namespace FG24