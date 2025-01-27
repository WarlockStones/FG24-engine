#include "Transform.hpp"
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <cstdio>

constexpr int TransformVersion = 1; // For serialization
bool FG24::Transform::WriteTo(FILE* file) const {
	/*
	// FILE* will be an already opened file stream pointer and now we just have
	// to write to it
	int version = TransformVersion;

	// TODO: Error checking. Did we write enough?
	std::fwrite(&version, sizeof(int), 1, file);
	std::fwrite(&location, sizeof(Vec3), 1, file);
	std::fwrite(&rotation, sizeof(Vec3), 1, file);
	std::fwrite(&scale, sizeof(Vec3), 1, file);
	return true;
	*/
	return false;
}

bool FG24::Transform::ReadFrom(FILE* file) {
	/*
	int version = 0;
	// TODO: Error checking. Did we read enough?
	std::fread(&version, sizeof(int), 1, file);
	std::fread(&location, sizeof(Vec3), 1, file);
	std::fread(&rotation, sizeof(Vec3), 1, file);
	std::fread(&scale,    sizeof(Vec3), 1, file);

	return true;
	*/
	return false;
}

// TODO: glm::vec3 lots of unintentional copies are being done
void FG24::Transform::SetLocation(glm::vec3 newLocation) {
	m_location = newLocation;
}

const glm::vec3& FG24::Transform::GetLocation() const {
	return m_location;
}

void FG24::Transform::SetScale(glm::vec3 newScale) {
	m_scale = newScale;
}

const glm::vec3& FG24::Transform::GetScale() const {
	return m_scale;
}

// x, y, z = pitch, yaw, roll
void FG24::Transform::SetRotation(glm::vec3 newEuler) {
	m_rotation = glm::quat(newEuler);
}

glm::vec3 FG24::Transform::GetRotationEuler() const {
	return glm::eulerAngles(m_rotation);
}

glm::mat4 FG24::Transform::GetRotationMatrix() const {
  return glm::mat4_cast(m_rotation); // This crashes
}
