#pragma once
#include "physics/Collider.hpp"
#include <glm/vec3.hpp>
#include <cstdio>

namespace FG24 {

class BoxCollider : public Collider {
public:
	BoxCollider() : Collider(ColliderType::Box) {
		std::printf("Warning! Box Collider did not initialise a Moment Of Inertia\n");
	};
	~BoxCollider() = default;

	glm::vec3 m_extents = glm::vec3(1);
	glm::vec3 m_center = glm::vec3(1);
};

} // namespace FG24