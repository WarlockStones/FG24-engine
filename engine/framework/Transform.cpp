#include "Transform.hpp"
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cstdio>

// constexpr int TransformVersion = 1; // For serialization
bool FG24::Transform::WriteTo(FILE* file) const {
    std::size_t n = 0;
	n += std::fwrite(&m_location, sizeof(glm::vec3), 1, file);
	n += std::fwrite(&m_rotation, sizeof(glm::quat), 1, file);
	n += std::fwrite(&m_scale,    sizeof(glm::vec3), 1, file);
	if (n != 3) {
		std::fprintf(stderr, "Error Transform did not write expected amount!\n");
		return false;
	}
	return true;
}

bool FG24::Transform::ReadFrom(FILE* file) {
    std::size_t n = 0;
	n += std::fread(&m_location, sizeof(glm::vec3), 1, file);
	n += std::fread(&m_rotation, sizeof(glm::quat), 1, file); // Save vec3 to decrease size
	n += std::fread(&m_scale,    sizeof(glm::vec3), 1, file);
	if (n != 3) {
		std::fprintf(stderr, "Error Transform did not read expected amount!\n");
		return false;
	}
	return true;
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
  return glm::mat4_cast(m_rotation); 
}

glm::mat4 FG24::Transform::GetModelMatrix() const {
	glm::mat4 tr =  glm::mat4(1);
	glm::mat4 rot = glm::mat4(1);
	glm::mat4 scl = glm::mat4(1);
	tr = glm::translate(tr, m_location);
	rot = GetRotationMatrix();
	scl = glm::scale(scl, m_scale);
	return tr * rot * scl;
}
