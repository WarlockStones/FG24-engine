#pragma once
#include <cstdint>
#include <string>
#include <variant>
#include "renderer/Mesh.hpp"
#include "renderer/MeshBlend.hpp"
#include "Transform.hpp"
#include "utils/Writable.hpp"

namespace FG24 {
class Material;

// An Entity is a thing in the game world
class Entity : public IWritable {
public:
	Entity() = default;
	explicit Entity(
		const Mesh* mesh,
		std::uint32_t shaderId,
		std::uint32_t textureId,
		const char* name);
	virtual ~Entity() = default;
	Entity(const Entity& other); // Copy constructor
	Entity& operator=(Entity&& other); // Move assignment
	void Draw() const;
    void DrawLightPass(std::uint32_t lightPassShaderId) const;

	bool WriteTo(FILE* file) const override;
	bool ReadFrom(FILE* file) override;

	const std::string& GetName() const;
	void SetName(const char* name);

	const Mesh* GetMesh() const;
	void SetMesh(const Mesh* mesh);

	Transform m_transform;
	std::uint32_t m_textureId = 0; // 0 means no texture
	std::uint32_t m_textureSpecularId = 0;
	bool m_drawAsWireframe = false;
private:
	const Mesh* m_mesh = nullptr;
	std::uint32_t m_shaderId{};
	std::string m_name = "Nil";
};
} // namespace FG24
