#pragma once
#include "physics/Collider.hpp"

namespace FG24 {
class SphereCollider final : public Collider {
public:
	SphereCollider() : Collider(ColliderType::Sphere) {
		// Calculate moment of inertia for a sphere
		float scalar = (2.0f / 3.0f) * m_mass * (m_radius * m_radius);
		m_momentOfInertia = glm::mat3(scalar);
	}
	~SphereCollider() = default;
	float m_radius = 1;
};

} // namespace FG24