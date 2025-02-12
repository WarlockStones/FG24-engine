#include "PhysicsSimulation.hpp"
#include "physics/Collider.hpp"
#include "physics/SphereCollider.hpp"
#include "physics/Collision.hpp"

namespace FG24 {
namespace PhysicsSimulation {

void ApplyGravity(const std::vector<Collider*>& colliders, float deltaTime) {
	for (Collider* c : colliders) {
		if (c->m_hasGravity) {
			constexpr float GravityMultiplier = -2;
			c->m_velocity += glm::vec3(0, GravityMultiplier, 0) * deltaTime;
		}
	}
}

std::vector<Collision> CheckIntersections(std::vector<Collider*>& colliders) {

	// Define collision intersections
	static auto Check = [](const Collider* c1, const Collider* c2) {
		switch (c1->m_type) {
		case ColliderType::Sphere:
		// Sphere sphere collision
		if (c2->m_type == ColliderType::Sphere) {
			const auto* s1 = dynamic_cast<const SphereCollider*>(c1);
			const auto* s2 = dynamic_cast<const SphereCollider*>(c2);

			float dist = glm::distance(s1->GetPosition(), s2->GetPosition());
			if (dist < s1->m_radius + s2->m_radius) {
				std::printf("SPHERE SPHERE INTERSECTION!\n");
				return true;
			}

			return false;
		}
		break;

		case ColliderType::Box:
		break;
		}

		return false;
	};

	// Generate collisions
	std::vector<Collision> collisions;
	for (Collider* c1 : colliders) {
		for (Collider* c2 : colliders) {
			if (c1 != c2 &&	Check(c1, c2)) {
				// Collision occurred
				collisions.emplace_back(*c1, *c2, glm::vec3(0,0,0));
			}
		}
	}

	return collisions;
}

void HandleCollisions(const std::vector<Collision>& collisions) {

}

void ApplyVelocity(std::vector<Collider*>& colliders, float deltaTime) {
	// Apply gravity
	for (Collider* c : colliders) {
		c->SetPosition(c->GetPosition() + c->m_velocity * deltaTime);
	}
}

} // namespace PhysicsSimulation
} // namespace FG24
