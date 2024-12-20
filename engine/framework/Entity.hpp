#pragma once
#include <cstdint>
#include "Transform.hpp"
#include "utils/Writable.hpp"

namespace FG24 {
class Material;
class Mesh;

// An Entity is a thing in the game world
class Entity : public IWritable {
public:
	Entity(Mesh* mesh, std::uint32_t shader, std::uint32_t texture = 0);
	~Entity() = default;
	void Draw();

	// TODO maybe virtual in the future?
	bool WriteTo(FILE* file) const override;
	bool ReadFrom(FILE* file) override;

	// TODO: Make private
	Transform transform;
private:
	Mesh* mesh{};
	std::uint32_t shader{};
	std::uint32_t texture = 0; // Texture id 0 means has no texture
};
} // namespace FG24
