#pragma once
#include <glm/glm.hpp>
#include "framework/Transform.hpp"

namespace FG24 {

enum class ColliderType {
	Sphere,
	Box,
};

// Abstract class only accessible through inheritance
class Collider {
public:
	Transform m_transform;
	const ColliderType m_type;

	// Move to dynamic body once that is added
	glm::vec3 m_velocity{0};
	float m_mass = 1;
	bool m_hasGravity = true;
	bool m_isStatic = false;
	
protected:
	// Restrict construction
	explicit Collider(ColliderType type);
	virtual ~Collider() = default;
	glm::vec3 m_center{ 0 };
};

} // namespace FG24