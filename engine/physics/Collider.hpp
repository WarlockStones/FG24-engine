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
	const ColliderType m_type;
	Transform m_transform;

	// Move to dynamic body once that is added
	bool m_hasGravity = true;
	glm::vec3 m_velocity{0};
	float m_mass{};
	
protected:
	// Restrict construction
	explicit Collider(ColliderType type);
	virtual ~Collider() = default;
	glm::vec3 m_center{ 0 };
};

} // namespace FG24