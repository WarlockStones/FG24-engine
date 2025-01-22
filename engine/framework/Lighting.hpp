#pragma once
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <vector>
#include "framework/Light.hpp"

namespace FG24 {

namespace Lighting {
constexpr int maxLights = 8;

constexpr glm::vec4 globalAmbient = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);

Light* CreateLight(
	glm::vec3 pos,
	LightType type,
	glm::vec4 diffuse,
	glm::vec4 specular);

void DeleteLight(Light* toDelete);

const std::vector<Light*>& GetLights();

} // namespace Lighting
} // namespace FG24
