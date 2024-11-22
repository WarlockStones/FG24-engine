#include "Material.hpp"

namespace FG24 {
Material::Material(Shader* shader, Texture* texture) {
	
}

const Shader& Material::GetShader() const {
	return *shader;
}
}