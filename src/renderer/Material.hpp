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

private:
	Shader* shader{};
	Texture* texture = nullptr;
};

}
