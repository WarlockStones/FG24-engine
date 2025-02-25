#pragma once
#include "physics/SphereCollider.hpp"
#include <glm/vec3.hpp>
#include "physics/Collision.hpp"

namespace FG24 {
class SphereCollider;
class BoxCollider;
namespace Intersect {
bool SphereSphere(const SphereCollider* s1, const SphereCollider* s2, Collision& out);
bool SphereBox(const SphereCollider* s, const BoxCollider* b, Collision& out);
bool BoxBox(const BoxCollider* b1, const BoxCollider* b2, Collision& out);

bool RayBox(const glm::vec3& origin, const glm::vec3& dir, const BoxCollider* b);
bool RaySphere(const glm::vec3& origin, const glm::vec3& dir, const SphereCollider* s);

} // namespace Intersections
} // namespace FG24
