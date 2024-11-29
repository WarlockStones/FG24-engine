#pragma once
#include <string>

struct Vec3 {
	float x = 0;
	float y = 0;
	float z = 0;
};

struct Entity {
	std::string name{};
	Vec3 position{};
	Vec3 rotation{};
	Vec3 scale{};
};
