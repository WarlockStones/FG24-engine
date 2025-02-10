#pragma once
#include "physics/Collider.hpp"
namespace FG24 {
struct Collision {
	Collision(const Collider& one, const Collider& two)
		: m_collider1(one), m_collider2(two) {

	}

	const Collider& m_collider1;
	const Collider& m_collider2;
};

} // namespace FG24