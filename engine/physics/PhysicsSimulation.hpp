#pragma once
#include <vector>
#include "physics/Collider.hpp"
#include "physics/Collision.hpp"

namespace FG24 {
namespace PhysicsSimulation {
void AddCollider(Collider* c);
const std::vector<Collider*>& GetColliders();
void ApplyGravity(float deltaTime);
std::vector<Collision> CheckIntersections();
void HandleCollisions(const std::vector<Collision>& collisions);

bool Raycast(
	const glm::vec3& origin,
	const glm::vec3& dir);
void ApplyVelocity(float deltaTime);
// Update visuals

} // namespace PhysicsSimulation
} // namespace FG24