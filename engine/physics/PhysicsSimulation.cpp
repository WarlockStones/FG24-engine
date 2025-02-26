#include "PhysicsSimulation.hpp"
#include <vector>
#include <optional>
#include "physics/Collision.hpp"
#include "physics/Collider.hpp"
#include "physics/SphereCollider.hpp"
#include "physics/BoxCollider.hpp"
#include "physics/Intersect.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <numbers>

namespace FG24 {

RayHit::RayHit(const glm::vec3& point, const Collider* collider, float distance) 
	: m_point(point), m_collider(collider), m_distance(distance) {
}

namespace PhysicsSimulation {
std::vector<Collider*> colliders;

void AddCollider(Collider* c) {
	colliders.push_back(c);
}

const std::vector<Collider*>& GetColliders() {
	return colliders;
}

void ApplyGravity(float deltaTime) {
	for (Collider* c : colliders) {
		if (c->m_hasGravity) {
			constexpr float GravityMultiplier = -2;
			c->m_velocity += glm::vec3(0, GravityMultiplier, 0) * deltaTime;
		}
	}
}

std::vector<Collision> CheckIntersections() {

	// Define collision intersections
	static auto Check = [](const Collider* c1, const Collider* c2, Collision& out) -> bool {
		if (c1->m_type == ColliderType::Sphere) {
			if (c2->m_type == ColliderType::Sphere) {
				const auto* s1 = dynamic_cast<const SphereCollider*>(c1);
				const auto* s2 = dynamic_cast<const SphereCollider*>(c2);
				return Intersect::SphereSphere(s1, s2, out);
			}

			if (c2->m_type == ColliderType::Box) {
				const auto* s = dynamic_cast<const SphereCollider*>(c1);
				const auto* b = dynamic_cast<const BoxCollider*>(c2);
				return Intersect::SphereBox(s, b, out);
			}
		}

		else if (c1->m_type == ColliderType::Box) {
			// Box Box Collision
			if (c2->m_type == ColliderType::Box) {
				const auto* box1 = dynamic_cast<const BoxCollider*>(c1);
				const auto* box2 = dynamic_cast<const BoxCollider*>(c2);
				return Intersect::BoxBox(box1, box2, out);
			}
		}

		return false;
	};

	// Generate collisions
	std::vector<Collision> collisions;
	for (Collider* c1 : colliders) {
		for (Collider* c2 : colliders) {
			if (c1 != c2) {
				Collision c(*c1, *c2);
				if (Check(c1, c2, c)) {
					// Collision occurred
					collisions.emplace_back(c);
				}
			}
		}
	}

	return collisions;
}

void HandleCollisions(const std::vector<Collision>& collisions) {
	for (const Collision& c : collisions) {
		auto& a = c.m_col1;
		auto& b = c.m_col2;
		if (a.m_isStatic && b.m_isStatic) {
			// Static Static
		// } else if (!a.m_isStatic && !b.m_isStatic) {
		} else if (true) {
			// Dynamic Dynamic
			glm::vec3 n = glm::normalize(
				b.m_transform.GetLocation() - a.m_transform.GetLocation());

			glm::vec3 relVel = b.m_velocity - a.m_velocity;

			float velocityAlongNormal = glm::dot(relVel, n);

			// Only update velocity if moving towards each other
			if (velocityAlongNormal < 0) { 
				constexpr float restitution = 0.1f;
				float impulse = (1 + restitution) * velocityAlongNormal;

				glm::vec3 impulseVector = impulse * n;
				if (!a.m_isStatic) {
					a.m_velocity += impulseVector;
					glm::vec3 r = c.m_point - a.m_transform.GetLocation();
					glm::vec3 torque = glm::cross(r, impulseVector);
					a.m_angularVelocity += a.m_inverseMomentOfInertia * torque;
				}
				if (!b.m_isStatic) {
					// Move in opposite direction
					b.m_velocity -= impulseVector;
					glm::vec3 r = c.m_point - b.m_transform.GetLocation();
					glm::vec3 torque = glm::cross(r, impulseVector);
					b.m_angularVelocity -= b.m_inverseMomentOfInertia * torque; // Rotate
				}
			}
		} else {
			/*                                                                      *
			 * DOES NOT WORK! Fix cube collision point, and cube moment of intertia *
			 *                                                                      *

			// Static Dynamic
			Collider& dynamicCol = a.m_isStatic ? b : a;
			// Collider& staticCollider = a.m_isStatic ? a : b;

			glm::vec3 n = glm::normalize(
				b.m_transform.GetLocation() - a.m_transform.GetLocation());
			glm::vec3 r = c.m_point - dynamicCol.m_transform.GetLocation();
			// glm::vec3 r = glm::vec3(0, 1, 0) - dynamicCol.m_transform.GetLocation();
			if (dynamicCol.m_type == ColliderType::Box) {
				std::printf("Box point = %f %f %f\n",
					c.m_point.x, c.m_point.y, c.m_point.z);
			}
			glm::vec3 v = dynamicCol.m_velocity + glm::cross(dynamicCol.m_angularVelocity, r);
			float vnDot = glm::dot(v, n);

			if (vnDot <= 0) {
				float invMass = (dynamicCol.m_mass > 0) ? 1.0f / dynamicCol.m_mass : 0;
				glm::vec3 rn = glm::cross(r, n);
				float angularEffect = glm::dot(
					rn,
					dynamicCol.m_inverseMomentOfInertia * rn);
				// BUG: inverseMomentOfInertia uninitialised
				constexpr float restitution = 0.1f;
				float impulseMag = -(1 + restitution) * vnDot / (invMass * angularEffect);
				glm::vec3 impulse = impulseMag * n;
				dynamicCol.m_velocity += impulse * invMass;
				dynamicCol.m_angularVelocity += 
					dynamicCol.m_inverseMomentOfInertia *
					glm::cross(r, impulse);

				// Sliding friction
				glm::vec3 tangentVel = v - (n * glm::dot(v, n));
				if (glm::length(tangentVel) > 0.0001f) {
					glm::vec3 frictionDir = -glm::normalize(tangentVel);
					constexpr float slidingFriction = 0.5f;
					glm::vec3 frictionImpulse = 
						frictionDir +
						slidingFriction *
						glm::length(tangentVel);

					dynamicCol.m_velocity += frictionImpulse * invMass;

					dynamicCol.m_angularVelocity += 
						dynamicCol.m_inverseMomentOfInertia *
						glm::cross(r, frictionImpulse);
				}
			} */ 
		}
	} // End of for each collision
}

std::optional<const RayHit> Raycast(const glm::vec3& origin, const glm::vec3& dir) {
	for (const Collider* c : colliders) {
		// Check intersect
		if ([origin, dir, c] {
			switch (c->m_type) {
			case ColliderType::Sphere:
			return Intersect::RaySphere(
				origin, dir, dynamic_cast<const SphereCollider*>(c));
			break;

			case ColliderType::Box:
			return Intersect::RayBox(
				origin, dir, dynamic_cast<const BoxCollider*>(c));

			default:
			return false; // No such ray intersect exists
			}
		}()) {
			// Intersect was successful
			return std::make_optional<const RayHit>(RayHit(
				// TODO: Calculate hit location and distance
				glm::vec3(0, 0, 0), 
				c,
				0.0f));
		}
	}

	return std::nullopt;
}

void ApplyVelocity(float deltaTime) {
	for (Collider* c : colliders) {
		if (c->m_isStatic) {
			c->m_velocity = { 0,0,0 };
			c->m_angularVelocity = { 0,0,0 };
		} else {
			c->m_transform.SetLocation(
				c->m_transform.GetLocation() + c->m_velocity * deltaTime);

			// Update rotation
			if (glm::length(c->m_angularVelocity) > 0.0001f) {
				c->m_transform.SetRotation(
					c->m_transform.GetRotationEuler() +
					(c->m_angularVelocity * deltaTime));
			}

			if (c->m_mass > 0) {
				constexpr float linearDrag = 0.32f;
				constexpr float angularDrag = 0.5f;
				c->m_velocity *= glm::pow(1.0f - linearDrag, deltaTime);
				c->m_angularVelocity *= glm::exp(-angularDrag * deltaTime);
			}

			glm::mat3 rot = c->m_transform.GetRotationMatrix();
			glm::mat inertiaTensorInWorldSpace = rot * c->m_momentOfInertia * glm::transpose(rot);
			c->m_inverseMomentOfInertia = glm::inverse(inertiaTensorInWorldSpace);
		}
	}
}

} // namespace PhysicsSimulation
} // namespace FG24
