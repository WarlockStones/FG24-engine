#pragma once
#include "utils/Writable.hpp"
#include "utils/containers/Vec3.hpp"
#include <glm/vec3.hpp>
#include <glm/ext/quaternion_float.hpp>

namespace FG24 {
class Transform : public IWritable {
public:
	bool WriteTo(FILE* file) const override;
	bool ReadFrom(FILE* file) override;

	void SetLocation(glm::vec3 newLocation);
	const glm::vec3& GetLocation() const;
	void SetRotation(glm::vec3 newEuler);
	const glm::quat& GetRotation() const;
	glm::vec3 GetRotationEuler() const;
	glm::mat4 GetRotationMatrix() const;
	glm::mat4 Transform::GetModelMatrix() const;
	void SetScale(glm::vec3 newScale);
	const glm::vec3& GetScale() const;

private:
	glm::vec3 m_location = {0.0f, 0.0f, 0.0f};
	glm::quat m_rotation{};
	glm::vec3 m_scale = {1.0f, 1.0f, 1.0f};
};
}
