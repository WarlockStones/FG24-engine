#pragma once
#include <glm/glm.hpp>

namespace FG24 {

class Collider {
public:
	virtual ~Collider() = default;

	glm::mat4 m_transform{ 0 };
	glm::vec3 m_center{ 0 };
	// Move to dynamic body once that is added
	bool m_hasGravity = true;
	glm::vec3 m_velocity{0};
	float m_mass{};
};

} // namespace FG24