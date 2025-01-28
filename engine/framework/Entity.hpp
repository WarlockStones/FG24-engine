#pragma once
#include <cstdint>
#include <string>
#include "Transform.hpp"
#include "utils/Writable.hpp"

namespace FG24 {
class Material;
class Mesh;

// An Entity is a thing in the game world
class Entity : public IWritable {
public:
	explicit Entity(
		const Mesh* mesh,
		std::uint32_t shaderId,
		std::uint32_t textureId,
		const char* name);
	virtual ~Entity() = default;
	Entity(const Entity& other); // Copy constructor
	Entity& operator=(Entity&& other); // Move assignment
	void Draw() const;

	bool WriteTo(FILE* file) const override;
	bool ReadFrom(FILE* file) override;

	const std::string& GetName() const;
	void SetName(const char* name);

	const Mesh& GetMesh() const;
	void SetMesh(const Mesh* mesh);

	Transform m_transform;
	std::uint32_t m_textureId = 0; // Texture m_ID 0 means has no m_textureID
private:
	const Mesh* m_mesh;
	std::uint32_t m_shaderId{};
	std::string m_name = "Nil";
};
} // namespace FG24
