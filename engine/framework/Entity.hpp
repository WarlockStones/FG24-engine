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
	explicit Entity(const Mesh& mesh, std::uint32_t shaderID, std::uint32_t textureID, int ID);
	~Entity() = default;
	Entity(const Entity& other); // Copy constructor
	Entity& operator=(Entity&& other); // Move assignment
	void Draw() const;
	int GetID() const;

	bool WriteTo(FILE* file) const override;
	bool ReadFrom(FILE* file) override;

	// TODO: Make private
	Transform m_transform;
private:
	const Mesh& m_mesh;
	std::uint32_t m_shaderID{};
	std::uint32_t m_textureID = 0; // Texture m_ID 0 means has no m_textureID
	int m_ID = -1;
};
} // namespace FG24
