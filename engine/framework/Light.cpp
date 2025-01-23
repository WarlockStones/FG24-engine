#include "Light.hpp"

namespace FG24 {
Light::Light(
	glm::vec3 position,
	LightType type,
	glm::vec4 diffuse,
	glm::vec4 specular,
	glm::vec3 attenuation,
	glm::vec3 rotation)
	: 
		m_position(position),
		m_type(type),
		m_diffuse(diffuse),
		m_specular(specular),
		m_attenuation(attenuation),
		m_rotation(rotation)
{
}

} // namespace FG24
