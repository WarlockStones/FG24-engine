#pragma once
#include <glm/glm.hpp>

namespace FG24 {

enum class ColliderType {
	Sphere,
	Box,
};

// Abstract class only accessible through inheritance
class Collider {
public:
	glm::vec3 GetPosition() const;
	void SetPosition(const glm::vec3& newPos);

	// Move to dynamic body once that is added
	bool m_hasGravity = true;
	glm::vec3 m_velocity{0};
	float m_mass{};
	const ColliderType m_type;
	
protected:
	// Restrict construction
	explicit Collider(ColliderType type);
	virtual ~Collider() = default;
	glm::mat4 m_transform{ 0 };
	glm::vec3 m_center{ 0 };
};

} // namespace FG24