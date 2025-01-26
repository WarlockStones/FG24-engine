#include "Camera.hpp"
#include <cstdio>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Globals.hpp"

namespace FG24 {
void Camera::Init(glm::vec3 position, float yaw, float pitch, std::uint32_t id) {
	m_position = position; 
	m_yaw = yaw;
	m_pitch = pitch;
	m_id = id;

	UpdateVectors();
}


void Camera::UpdateVectors() {
	glm::vec3 dir;
	dir.x = glm::cos(glm::radians(m_yaw)) * glm::cos(glm::radians(m_pitch));
	dir.y = glm::sin(glm::radians(m_pitch));
	dir.z = glm::sin(glm::radians(m_yaw)) * glm::cos(glm::radians(m_pitch));
	m_front = glm::normalize(dir);
	m_right = glm::normalize(glm::cross(m_front, g_worldUp));
	m_up = glm::normalize(glm::cross(m_right, m_front));
}

glm::mat4 Camera::GetViewMatrix() const {
	// Move the entire scene around inversed to where we want camera to move
	// OpenGL is right-handed system so +x = right, +y = up. +z = backwards.
	// This is why forward is -z. 
	return glm::lookAt(m_position, m_position + m_front, m_up);
}

void Camera::Update(float deltaTime) {
	// Camera pitch and yaw movement
    if (g_xRelativeMouseMotion != 0 || g_yRelativeMouseMotion != 0) {
		m_yaw += (g_xRelativeMouseMotion * m_mouseSensitivity);
		m_pitch += (g_yRelativeMouseMotion * m_mouseSensitivity * -1); // -1 to reverse up
		glm::clamp(m_pitch, -89.0f, 89.0f);
		UpdateVectors();

		// Reset mouse move when used
		g_xRelativeMouseMotion = 0; 
		g_yRelativeMouseMotion = 0;
	}

	// Move position. WASD movement
	if (g_yvel != 0 || g_xvel != 0) {
		m_position += (m_movementSpeed * m_front) * g_yvel * static_cast<float>(deltaTime);
		m_position += 
			(glm::normalize(glm::cross(m_front, m_up)) * m_movementSpeed) *
			g_xvel *
			static_cast<float>(deltaTime);
	}
}

glm::vec3 Camera::GetPosition() const {
	return m_position;
}

float Camera::GetYaw() const {
	return m_yaw;
}

float Camera::GetPitch() const {
	return m_pitch;
}
} // namespace FG24
