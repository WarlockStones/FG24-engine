#pragma once
#include <glm/vec3.hpp>

namespace FG24 {
class Collider;

struct RayHit {
	RayHit(const glm::vec3& point, const Collider* collider, float distance);
	const glm::vec3 m_point;
	const Collider* m_collider = nullptr;
	const float m_distance;
};

} // namespace FG24