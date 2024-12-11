#include "Transform.hpp"
#include <cstdio>

constexpr int TransformVersion = 1; // For serialization
bool FG24::Transform::WriteTo(FILE* file) const {
	// FILE* will be an already opened file stream pointer and now we just have
	// to write to it
	int version = TransformVersion;

	// TODO: Error checking. Did we write enough?
	std::fwrite(&version, sizeof(int), 1, file);
	std::fwrite(&location, sizeof(Vec3), 1, file);
	std::fwrite(&rotation, sizeof(Vec3), 1, file);
	std::fwrite(&scale, sizeof(Vec3), 1, file);
	return true;
}

bool FG24::Transform::ReadFrom(FILE* file) {
	int version = 0;
	// TODO: Error checking. Did we read enough?
	std::fread(&version, sizeof(int), 1, file);
	std::fread(&location, sizeof(Vec3), 1, file);
	std::fread(&rotation, sizeof(Vec3), 1, file);
	std::fread(&scale,    sizeof(Vec3), 1, file);

	return true;
}
