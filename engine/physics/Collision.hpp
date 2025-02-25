#pragma once
#include <glm/vec3.hpp>
#include "physics/Collider.hpp"
namespace FG24 {
struct Collision {
	Collision(Collider& col1, Collider& col2)
		: m_col1(col1), m_col2(col2) {
	}

	glm::vec3 m_normal;
	glm::vec3 m_point; // Point of collision
	Collider& m_col1;
	Collider& m_col2;
};

} // namespace FG24
