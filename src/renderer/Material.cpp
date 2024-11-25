#include "Material.hpp"
#include <cstdint>
#include "Shader.hpp"
#include <cassert>
#include "Texture.hpp"

#include <cstdio>

namespace FG24 {
Material::Material(Shader* shader, Texture* texture) : 
	shader(shader), texture(texture) {
}

std::uint32_t Material::GetShader() const {
	assert(shader); // Shader should never be null
	return shader->GetShaderProgram();
}

std::uint32_t Material::GetTexture() const {
	if (texture) {
		return texture->Get();
	} else {
		return 0;
	}
}
} // namespace FG24
