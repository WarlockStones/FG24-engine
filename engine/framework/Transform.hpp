#pragma once
#include "utils/Writable.hpp"
#include "utils/containers/Vec3.hpp"

namespace FG24 {
class Transform : public IWritable {
public:
	bool WriteTo(FILE* file) const override;
	bool ReadFrom(FILE* file) override;

	// TODO make private
	Vec3 location{};
	Vec3 rotation{};
	Vec3 scale{};
};
}