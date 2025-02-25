#pragma once
#include "physics/Collider.hpp"
namespace glm { typedef vec3; }
namespace FG24 {
struct Collision {
	Collision(Collider& col1, Collider& col2, const glm::vec3& colPoint, const glm::vec3& normal)
		: m_col1(col1), m_col2(col2), m_point(colPoint), m_normal(normal){
	}

	const glm::vec3 m_normal;
	const glm::vec3 m_point; // Point of collision
	Collider& m_col1;
	Collider& m_col2;
};

} // namespace FG24