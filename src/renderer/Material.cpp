#include "Material.hpp"
#include <cstdint>
#include "Shader.hpp"
#include <cassert>

#include <cstdio>

namespace FG24 {
Material::Material(Shader* shader, Texture* texture) : 
	shader(shader), texture(texture) {
	
}

std::uint32_t Material::GetShader() const {
	assert(shader); // Shader should never be null
	return shader->GetShaderProgram();
}
} // namespace FG24
