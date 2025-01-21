#pragma once
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>

namespace FG24 {
namespace Lighting {
constexpr int maxLights = 8;
enum LightType { Point, Directional, Spot };

struct Light {
	Light::Light(
		glm::vec3 pos,
		LightType type,
		glm::vec4 diffuse,
		glm::vec4 specular,
		int m_ID);
	glm::vec3 m_position{0};
	LightType m_type = Point;
	glm::vec4 m_diffuse{1};
	glm::vec4 m_specular{1};
	int m_id;
};
constexpr glm::vec4 globalAmbient = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);

int AddLight();
void RemoveLight(std::uint32_t m_ID);

} // namespace Lighting
} // namespace FG24
