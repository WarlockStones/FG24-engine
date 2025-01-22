#include "Light.hpp"

namespace FG24 {
Light::Light(glm::vec3 position, LightType type, glm::vec4 diffuse, glm::vec4 specular)
	: m_position(position), m_type(type), m_diffuse(diffuse), m_specular(specular) {
}

} // namespace FG24