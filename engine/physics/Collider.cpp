#include "Collider.hpp"
#include "Globals.hpp"

namespace FG24 {
Collider::Collider(ColliderType type) : m_type(type) {
}

glm::vec3 Collider::GetPosition() const {
	return glm::vec3(m_transform[3]);
}

void Collider::SetPosition(const glm::vec3& newPos) {
	m_transform[3] = glm::vec4(newPos, 1.0f);
}

} // namespace FG24