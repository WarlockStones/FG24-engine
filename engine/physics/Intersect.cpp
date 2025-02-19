#include "Intersect.hpp"
#include <glm/glm.hpp>
#include "physics/Collider.hpp"
#include "physics/SphereCollider.hpp"
#include "physics/BoxCollider.hpp"
#include "physics/Ray.hpp"

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

	return false;
}

bool RaySphere(const glm::vec3& origin, const glm::vec3& dir, const SphereCollider* s) {

	return false;
}

} // namespace Intersect
} // namespace FG24
