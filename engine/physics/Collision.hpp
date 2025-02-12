#pragma once
#include "physics/Collider.hpp"
namespace glm { typedef vec3; }
namespace FG24 {
struct Collision {
	Collision(const Collider& col1, const Collider& col2, const glm::vec3& colPoint)
		: m_col1(col1), m_col2(col1), m_point(colPoint){

	}

	const Collider& m_col1;
	const Collider& m_col2;
	const glm::vec3 m_point; // Point of collision
};

} // namespace FG24