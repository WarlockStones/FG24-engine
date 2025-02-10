#include "PhysicsSimulation.hpp"
#include "physics/Collider.hpp"
#include "physics/Collision.hpp"

namespace FG24 {
namespace PhysicsSimulation {

static bool CheckIntersection(Collider* c1, Collider* c2) {

	return true;
}

void ApplyVelocity(std::vector<Collider*>& colliders, float deltaTime) {
	// Apply gravity
	for (Collider* c : colliders) {
	}

}

std::vector<Collision> CheckIntersections(std::vector<Collider*> colliders) {
	std::vector<Collision> collisions;
	for (auto* c1 : colliders) {
		for (auto c2 : colliders) {
			if (c1 != c2 &&	CheckIntersection(c1, c2)) {
				// Collision occurred
				collisions.emplace_back(*c1, *c2);
			}
		}
	}

	return std::vector<Collision>();
}

void HandleCollisions(const std::vector<Collision>& collisions) {

}

} // namespace PhysicsSimulation
} // namespace FG24
