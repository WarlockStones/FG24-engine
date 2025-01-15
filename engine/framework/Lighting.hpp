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
		glm::vec4 specular);
	glm::vec3 position{0};
	LightType type = Point;
	glm::vec4 diffuse{1};
	glm::vec4 specular{1};
};
	glm::vec4 globalAmbient = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);

	int AddLight(Light light);
	void RemoveLight(std::uint32_t id);

} // namespace Lighting
} // namespace FG24
