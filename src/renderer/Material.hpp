#pragma once
#include <cstdint>


namespace FG24 {
class Shader;
class Texture;

// Material is a shader with an optional texture
class Material {
public:
	Material(Shader* shader, Texture* texture = nullptr);

	std::uint32_t GetShader() const;
	std::uint32_t GetTexture() const;

private:
	Shader* shader{};
	Texture* texture = nullptr;
};

}
