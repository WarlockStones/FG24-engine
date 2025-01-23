#pragma once
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
namespace FG24 {
enum LightType { Point, Spot, Directional};
struct Light {
	Light(
		glm::vec3 pos,
		LightType type,
		glm::vec4 diffuse,
		glm::vec4 specular,
		glm::vec3 attenuation);
	glm::vec3 m_position{0};
	int m_type = LightType::Point; // int because that is what GLSL wants
	glm::vec4 m_diffuse{1}; // diffuseColor?
	glm::vec4 m_specular{1}; // specularColor?
	glm::vec3 m_attenuation{};
	bool enabled = true;
};

} // namespace FG24
