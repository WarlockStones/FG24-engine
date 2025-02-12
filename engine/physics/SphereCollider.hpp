#pragma once
#include "physics/Collider.hpp"

namespace FG24 {
class SphereCollider final : public Collider {
public:
	SphereCollider() : Collider(ColliderType::Sphere) {
	}
	~SphereCollider() = default;
	float m_radius = 1;
};

} // namespace FG24