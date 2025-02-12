#pragma once
#include <vector>
#include "physics/Collider.hpp"
#include "physics/Collision.hpp"

namespace FG24 {
namespace PhysicsSimulation {
void ApplyGravity(const std::vector<Collider*>& colliders, float deltaTime);
std::vector<Collision> CheckIntersections(std::vector<Collider*>& colliders);
void HandleCollisions(const std::vector<Collision>& collisions);
void ApplyVelocity(std::vector<Collider*>& colliders, float deltaTime);
// Update visuals

} // namespace PhysicsSimulation
} // namespace FG24