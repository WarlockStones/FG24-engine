#pragma once
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace FG24 {
class Camera {
public:
	Camera(glm::vec3 a_position, glm::vec3 a_front, glm::vec3 a_up);
	glm::mat4 GetViewMatrix() const;
	void Update(float deltaTime);
	glm::vec3 GetPosition() const;
private:
	void UpdateCameraVectors();
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	float pitch{};
	float yaw{};
	float movementSpeed = 2.0f;
	float mouseSensitivity = 0.1f;
	float fov = 45.0f;
	float xVel{};
	float yVel{};
	bool mouseMotion = false;
};
} // namespace FG24
