#include "Lighting.hpp"
#include <Vector>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace FG24 {
namespace Lighting {
std::vector<Light> lights;

std::vector<Light>::iterator GetLight(std::uint32_t m_ID) {
	return std::find_if(lights.begin(), lights.end(),
		[&](Light& light) {
		return light.m_id = m_ID;
	});
}

Light::Light(glm::vec3 position, LightType type, glm::vec4 diffuse, glm::vec4 specular, int m_ID)
	: m_position(position), m_type(type), m_diffuse(diffuse), m_specular(specular), m_id(m_ID) {
}

// Add a light and return an m_ID
[[nodiscard]] int AddLight(
	glm::vec3 pos,
	LightType type,
	glm::vec4 diffuse,
	glm::vec4 specular)
{
	static int freeID = 0;
	lights.emplace_back(Light(pos, type, diffuse, specular, freeID++)); 
	return --freeID;
}
void RemoveLight(std::uint32_t m_ID) {
	auto l = GetLight(m_ID);
	if (l != lights.end()) {
		lights.erase(l);
	}
}
} // namespace Lighting
} // namespace FG24