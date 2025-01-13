#include "Camera.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Globals.hpp"

// TODO:
FG24::Camera::Camera(glm::vec3 a_position, glm::vec3 a_front, glm::vec3 a_up) 
	: position(a_position), front(a_front), up(a_up) {

	// Constant WorldUp (0 1 0)
	// Constant WorldForward (0 0 -1)
	// This camera constructor initializes by pitch and yaw.
	// Care learnopengl.com naming convention. front != Front. WorldUp = up (camera parameter) lol...
	// WorldUp and Forward should be in global probably
	
	// Initialize pitch and yaw from front
	// Or maybe just live with having to set pitch and yaw as inital values

	glm::quat
	
	// UpdateCameraVectors();
	std::printf("front: %f %f %f\n", front.x, front.y, front.z);
}

void FG24::Camera::UpdateCameraVectors() {
	glm::vec3 dir;
	dir.x = glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
	dir.y = glm::sin(glm::radians(pitch));
	dir.z = glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
	front = glm::normalize(dir);
}

glm::mat4 FG24::Camera::GetViewMatrix() const {
	return glm::lookAt(position, position + front, up);
}

void FG24::Camera::Update(float deltaTime) {
	// Look for inputs and update movements 

	// lock pitch for a more common FPS camera

	std::printf("Update: front: %f %f %f\n", front.x, front.y, front.z);

	// Only do calculation if there has been mouse movement
	if (g_mouseMotion) {
		g_mouseMotion = false;
		yaw += (g_xRel * mouseSensitivity);
		pitch += (g_yRel * mouseSensitivity * -1); // -1 to reverse up
		glm::clamp(pitch, -89.0f, 89.0f);
		UpdateCameraVectors();
	}

	// Move position. WASD
	position += (movementSpeed * front) * g_yvel * static_cast<float>(deltaTime);
	position += 
		(glm::normalize(glm::cross(front, up)) * movementSpeed)	* 
		g_xvel *
		static_cast<float>(deltaTime);
}

glm::vec3 FG24::Camera::GetPosition() const {
	return position;
}
