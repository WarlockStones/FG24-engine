#pragma once


namespace FG24 {
class Shader;
class Texture;

// Material is a shader with an optional texture
class Material {
public:
	Material(Shader* shader, Texture* texture = nullptr);

	const Shader& GetShader() const;

private:
	Shader* shader{};
	Texture* texture = nullptr;
};

}