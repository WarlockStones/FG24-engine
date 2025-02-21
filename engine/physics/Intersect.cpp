#include "Intersect.hpp"
#include <glm/glm.hpp>
#include "physics/Collider.hpp"
#include "physics/SphereCollider.hpp"
#include "physics/BoxCollider.hpp"
#include "physics/Ray.hpp"
#include <algorithm>

namespace FG24 {
namespace Intersect {

bool SphereSphere(const SphereCollider* s1, const SphereCollider* s2) {
	float dist = glm::distance(
		s1->m_transform.GetLocation(),
		s2->m_transform.GetLocation());
	if (dist < s1->m_radius + s2->m_radius) {
		std::printf("Sphere Sphere intersection!\n");
		return true;
	}

	return false;
}

bool SphereBox(const SphereCollider* s, const BoxCollider* b) {
	glm::mat4 boxTra = b->m_transform.GetModelMatrix();
	glm::vec4 sphereCent = glm::vec4(s->m_transform.GetLocation(), 1);
	glm::mat4 inv = glm::inverse(boxTra);
	glm::vec3 localSphereCenter = inv * sphereCent;
	glm::vec3 closestPoint = glm::clamp(
		localSphereCenter,
		-b->m_extents,
		b->m_extents);
	float dist = glm::length(localSphereCenter - closestPoint);
	if (dist < s->m_radius * s->m_radius) {
		std::printf("Sphere Box intersection!\n");
		return true;
	} 

	return false;
}

bool BoxBox(const BoxCollider* box1, const BoxCollider* box2) {
	// Put one cube in the other's local space
	glm::mat3 rot1 = glm::mat3(box1->m_transform.GetModelMatrix());
	glm::mat3 rot2 = glm::mat3(box2->m_transform.GetModelMatrix());
	glm::vec3 translation = 
		glm::vec3(box2->m_transform.GetLocation()) -
		glm::vec3(box1->m_transform.GetLocation());
	translation = glm::transpose(rot1) * translation;
	
	glm::mat3 rot = glm::transpose(rot1) * rot2;
	glm::mat3 absRot = glm::mat3(
		glm::abs(rot[0]), glm::abs(rot[1]), glm::abs(rot[2]));

	// Separating Axis Theorem to detect if intersection impossible
	for (int i = 0; i < 3; ++i) {
		float ra = box1->m_extents[i];
		float rb = glm::dot(absRot[i], box2->m_extents);
		if (glm::abs(translation[i]) > ra + rb) {
			return false;
		}
	}

	for (int i = 0; i < 3; ++i) {
		float ra = glm::dot(absRot[i], box1->m_extents);
		float rb = box2->m_extents[i];
		if (glm::abs(glm::dot(rot[i], translation)) > ra + rb){
			return false;
		}
	}

	// Separating Axis Theorem passed, boxes must be intersecting
	std::printf("Box Box Intersection\n");
	return true;
}

bool RayBox(const glm::vec3& origin, const glm::vec3& dir, const BoxCollider* b) {
	// Make box local space
	glm::vec3 center = glm::vec3(b->m_transform.GetLocation());
	glm::mat3 rot = b->m_transform.GetRotationMatrix();

	glm::vec3 localOrigin = glm::transpose(rot) * (origin - center);
	glm::vec3 localDir = glm::transpose(rot) * dir;

	BoxCollider localBox;
	localBox.m_center = glm::vec3(0, 0, 0);
	localBox.m_extents = b->m_extents;

	// Do intersection check
	const glm::vec3& localBoxLoc = localBox.m_transform.GetLocation();

	glm::vec3 min = localBoxLoc - localBox.m_extents;
	glm::vec3 max = localBoxLoc + localBox.m_extents;

	// BUG: Vector of 0 results in a division of 0 resulting in an 'inf' value
	glm::vec3 invDir = 1.0f / dir;

	float t1 = (min.x - origin.x) * invDir.x;
	float t2 = (max.x - origin.x) * invDir.x;
	float t3 = (min.y - origin.y) * invDir.y;
	float t4 = (max.y - origin.y) * invDir.y;
	float t5 = (min.z - origin.z) * invDir.z;
	float t6 = (max.z - origin.z) * invDir.z;

	float tMin = std::max(std::max(std::min(t1, t2), std::min(t3, t4)), std::min(t5, t6));
	float tMax = std::min(std::min(std::max(t1, t2), std::max(t3, t4)), std::max(t5, t6));

	return tMax >= std::max(0.0f, tMin);
}

bool RaySphere(const glm::vec3& origin, const glm::vec3& dir, const SphereCollider* s) {

	glm::vec3 originToSphere = s->m_transform.GetLocation() - origin;
	float t0 = glm::dot(originToSphere, dir);
	float distSq = glm::dot(originToSphere, originToSphere) - t0 * t0;
	float radSq = s->m_radius * s->m_radius;
	if (distSq > radSq) {
		return false;
	}

	float t1 = glm::sqrt(radSq - distSq);
	constexpr float eps = 0.000001f;
	float intersectDist = (t1 > t1 + eps) ? t0 - t1 : t0 + t1;

	return intersectDist > eps;
}

} // namespace Intersect
} // namespace FG24
