#include "Ray.hpp"
#include <glm/vec3.hpp>
#include "Collider.hpp"

namespace FG24 {

RayHit::RayHit(const glm::vec3& point, const Collider* collider, float distance) 
	: m_point(point), m_collider(collider), m_distance(distance) {
}

} // namespace FG24
