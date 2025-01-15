#include "Lighting.hpp"
#include <Vector>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace FG24 {
namespace Lighting {

// Lights could be stored in an array of lights.
// But you must find a way to store and use available indicies. Coz you can
// add and remove. 
// There is probably a smart way of doing this
// But that would require the array to be initialized on start, and only flag
// the indicies that are in use. hmmm....

// I cant use a vector either.... Since it will reorder the array when I 
// remove elements from it. So the id will point to other Lights if it just
// points to the index. There must be a way to do this without dynamic alloc.

std::vector<Light> lights;

Light::Light(
	glm::vec3 pos,
	LightType type,
	glm::vec4 diffuse,
	glm::vec4 specular)
	: position(pos), type(type), diffuse(diffuse), specular(specular)
{
}

// Add a light and return an id
[[nodiscard]] int AddLight(
	glm::vec3 pos,
	LightType type,
	glm::vec4 diffuse,
	glm::vec4 specular)
{
	// TODO: Read Tour of C++
	// Which and why? 
	// Emplace + std::move?
	// Emplace + construct in argument?
	// Push_back + construct in argument?
	lights.emplace_back(Light(pos, type, diffuse, specular)); 
	return lights.size() - 1;
}
void RemoveLight(std::uint32_t id) {
	lights.erase(lights.begin() + id);
}
Light* GetLight(std::uint32_t id) {
	// if ()
	return nullptr;
}
} // namespace Lighting
} // namespace FG24