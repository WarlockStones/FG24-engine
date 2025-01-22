#pragma once
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
namespace FG24 {
enum class LightType { Point, Directional, Spot };
struct Light {
	Light::Light(
		glm::vec3 pos,
		LightType type,
		glm::vec4 diffuse,
		glm::vec4 specular);
	glm::vec3 m_position{0};
	LightType m_type = LightType::Point;
	glm::vec4 m_diffuse{1}; // diffuseColor?
	glm::vec4 m_specular{1}; // specularColor?
};

} // namespace FG24
