#include "PhysicsSimulation.hpp"
#include <vector>
#include "physics/Collision.hpp"
#include "physics/Collider.hpp"
#include "physics/SphereCollider.hpp"
#include "physics/BoxCollider.hpp"
#include "physics/Intersect.hpp"

namespace FG24 {
namespace PhysicsSimulation {
std::vector<Collider*> colliders;

void AddCollider(Collider* c) {
	colliders.push_back(c);
}

const std::vector<Collider*>& GetColliders() {
	return colliders;
}

void ApplyGravity(float deltaTime) {
	for (Collider* c : colliders) {
		if (c->m_hasGravity) {
			constexpr float GravityMultiplier = -2;
			c->m_velocity += glm::vec3(0, GravityMultiplier, 0) * deltaTime;
		}
	}
}

std::vector<Collision> CheckIntersections() {

	// Define collision intersections
	static auto Check = [](const Collider* c1, const Collider* c2) {
		if (c1->m_type == ColliderType::Sphere) {
			if (c2->m_type == ColliderType::Sphere) {
				const auto* s1 = dynamic_cast<const SphereCollider*>(c1);
				const auto* s2 = dynamic_cast<const SphereCollider*>(c2);
				return Intersect::SphereSphere(s1, s2);
			}

			if (c2->m_type == ColliderType::Box) {
				const auto* s = dynamic_cast<const SphereCollider*>(c1);
				const auto* b = dynamic_cast<const BoxCollider*>(c2);
				return Intersect::SphereBox(s, b);
			}
		}

		else if (c1->m_type == ColliderType::Box) {
			// Box Box Collision
			if (c2->m_type == ColliderType::Box) {
				const auto* box1 = dynamic_cast<const BoxCollider*>(c1);
				const auto* box2 = dynamic_cast<const BoxCollider*>(c2);
				return Intersect::BoxBox(box1, box2);
			}
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
	// Do the responses
}

bool Raycast(const glm::vec3& origin, const glm::vec3& dir) {
	return false;
}


void ApplyVelocity(float deltaTime) {
	// Apply gravity
	for (Collider* c : colliders) {
		c->m_transform.SetLocation(
			c->m_transform.GetLocation() + c->m_velocity * deltaTime);
	}
}

} // namespace PhysicsSimulation
} // namespace FG24
