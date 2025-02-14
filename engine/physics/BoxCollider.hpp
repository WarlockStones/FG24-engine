#pragma once
#include "physics/Collider.hpp"
#include <glm/vec3.hpp>

namespace FG24 {

class BoxCollider : public Collider {
public:
	BoxCollider() : Collider(ColliderType::Box) {
	};
	~BoxCollider() = default;

	glm::vec3 m_extents = glm::vec3(1);
	glm::vec3 m_center = glm::vec3(1);
};

} // namespace FG24