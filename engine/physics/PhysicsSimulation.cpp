#include "PhysicsSimulation.hpp"
#include "physics/Collision.hpp"
#include "physics/Collider.hpp"
#include "physics/SphereCollider.hpp"
#include "physics/BoxCollider.hpp"

namespace FG24 {
namespace PhysicsSimulation {

void ApplyGravity(const std::vector<Collider*>& colliders, float deltaTime) {
	for (Collider* c : colliders) {
		if (c->m_hasGravity) {
			constexpr float GravityMultiplier = -2;
			c->m_velocity += glm::vec3(0, GravityMultiplier, 0) * deltaTime;
		}
	}
}

std::vector<Collision> CheckIntersections(std::vector<Collider*>& colliders) {

	// Define collision intersections
	static auto Check = [](const Collider* c1, const Collider* c2) {
		if (c1->m_type == ColliderType::Sphere) {
			// Sphere Sphere collision
			if (c2->m_type == ColliderType::Sphere) {
				const auto* s1 = dynamic_cast<const SphereCollider*>(c1);
				const auto* s2 = dynamic_cast<const SphereCollider*>(c2);

				float dist = glm::distance(
					s1->m_transform.GetLocation(), s2->m_transform.GetLocation());
				if (dist < s1->m_radius + s2->m_radius) {
					std::printf("Sphere Sphere intersection!\n");
					return true;
				}

				return false;
			}

			// Sphere Box Collision
			if (c2->m_type == ColliderType::Box) {
				const auto* s1 = dynamic_cast<const SphereCollider*>(c1);
				const auto* b = dynamic_cast<const BoxCollider*>(c2);

				glm::mat4 boxTra = b->m_transform.GetModelMatrix();
				glm::vec4 sphereCent = glm::vec4(s1->m_transform.GetLocation(), 1);
				glm::mat4 inv = glm::inverse(boxTra);
				glm::vec3 localSphereCenter = inv * sphereCent;
				glm::vec3 closestPoint = glm::clamp(
					localSphereCenter,
					-b->m_extents,
					b->m_extents);
				float dist = glm::length(localSphereCenter - closestPoint);
				if (dist < s1->m_radius * s1->m_radius) {
					std::printf("Sphere Box intersection!\n");
					return true;
				} else {
					return false;
				}
			}
		}

		else if (c1->m_type == ColliderType::Box) {
			// Box Box Collision
			if (c2->m_type == ColliderType::Box) {
				const auto* box1 = dynamic_cast<const BoxCollider*>(c1);
				const auto* box2 = dynamic_cast<const BoxCollider*>(c2);

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

}

void ApplyVelocity(std::vector<Collider*>& colliders, float deltaTime) {
	// Apply gravity
	for (Collider* c : colliders) {
		c->m_transform.SetLocation(
			c->m_transform.GetLocation() + c->m_velocity * deltaTime);
	}
}

} // namespace PhysicsSimulation
} // namespace FG24
