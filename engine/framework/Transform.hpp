#pragma once
#include "utils/Writable.hpp"
#include "utils/containers/Vec3.hpp"
#include <glm/vec3.hpp>

namespace FG24 {
class Transform : public IWritable {
public:
	bool WriteTo(FILE* file) const override;
	bool ReadFrom(FILE* file) override;

	// TODO make private
	glm::vec3 location{};
	glm::vec3 rotation{};
	glm::vec3 scale{};
};
}