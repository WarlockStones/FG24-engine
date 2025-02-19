#pragma once
#include "physics/SphereCollider.hpp"

namespace glm { typedef vec3; }
namespace FG24 {
class SphereCollider;
class BoxCollider;
namespace Intersect {
bool SphereSphere(const SphereCollider* s1, const SphereCollider* s2);
bool SphereBox(const SphereCollider* s, const BoxCollider* b);
bool BoxBox(const BoxCollider* b1, const BoxCollider* b2);
bool RayBox(const glm::vec3& origin, const glm::vec3& dir, const BoxCollider* b);
bool RaySphere(const glm::vec3& origin, const glm::vec3& dir, const SphereCollider* s);

} // namespace Intersections
} // namespace FG24