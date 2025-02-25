#pragma once
#include "physics/SphereCollider.hpp"
#include <optional>
#include <glm/vec3.hpp>
#include "physics/Collision.hpp"

namespace FG24 {
class SphereCollider;
class BoxCollider;
namespace Intersect {
std::optional<Collision> SphereSphere(const SphereCollider* s1, const SphereCollider* s2);
std::optional<Collision> SphereBox(const SphereCollider* s, const BoxCollider* b);
std::optional<Collision> BoxBox(const BoxCollider* b1, const BoxCollider* b2);
bool RayBox(const glm::vec3& origin, const glm::vec3& dir, const BoxCollider* b);
bool RaySphere(const glm::vec3& origin, const glm::vec3& dir, const SphereCollider* s);

} // namespace Intersections
} // namespace FG24
