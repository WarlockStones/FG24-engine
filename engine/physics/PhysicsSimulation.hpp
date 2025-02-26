#pragma once
#include <vector>
#include <optional>
#include "physics/Collider.hpp"
#include "physics/Collision.hpp"

namespace FG24 {
struct RayHit {
	RayHit(const glm::vec3& point, const Collider* collider, float distance);
	const glm::vec3 m_point;
	const Collider* m_collider = nullptr;
	const float m_distance;
};

namespace PhysicsSimulation {
void AddCollider(Collider* c);
const std::vector<Collider*>& GetColliders();
void ApplyGravity(float deltaTime);
std::vector<Collision> CheckIntersections();
void HandleCollisions(const std::vector<Collision>& collisions);

std::optional<const RayHit> Raycast(
	const glm::vec3& origin,
	const glm::vec3& dir);
void ApplyVelocity(float deltaTime);
} // namespace PhysicsSimulation
} // namespace FG24