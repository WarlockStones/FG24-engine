#include "Lighting.hpp"
#include <Vector>
#include <cassert>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace FG24 {
namespace Lighting {
std::vector<Light*> lights;

// Add a light and return an m_ID
[[nodiscard]] Light* CreateLight(
	glm::vec3 pos,
	LightType type,
	glm::vec4 diffuse,
	glm::vec4 specular)
{
	if (lights.size() < maxLights) {
		Light* l = new Light(pos, type, diffuse, specular);
		lights.push_back(l);
		return l;
	} else {
		std::fprintf(stderr, "Error max lights reached! (max lights: %d)", maxLights);
		return nullptr;
	}
}

void DeleteLight(Light* toDelete) {
	assert(toDelete != nullptr);
	for (int i = 0; i < lights.size(); ++i) {
		if (lights[i] == toDelete) {
			delete lights[i];
			lights.erase(lights.begin() + i);
		}
	}
}

const std::vector<Light*>& GetLights() {
	return lights;
}
} // namespace Lighting
} // namespace FG24