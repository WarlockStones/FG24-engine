#pragma once
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace FG24 {
// The camera class is to be created from the Camera Manager
enum class ProjectionType { Perspective, Orthographic };
class Camera {
public:
    Camera() = default;
    ~Camera() = default;
    void Init(glm::vec3 position, float yaw, float pitch, std::uint32_t id);
    Camera& operator=(Camera&& other);
	glm::mat4 GetViewMatrix() const;
	void Update(float deltaTime);
	void SetPosition(float x, float y, float z);
	const glm::vec3& GetPosition() const;
	void SetYaw(float yaw);
	float GetYaw() const;
	void SetPitch(float pitch);
	float GetPitch() const;
	std::uint32_t m_id; 
	float m_fov = 45.0f;
	ProjectionType projection = ProjectionType::Perspective;
	void UpdateVectors();
private:
	glm::vec3 m_position;
	glm::vec3 m_front;
    glm::vec3 m_right;
    glm::vec3 m_up;
	float m_yaw{};
	float m_pitch{};
	float m_movementSpeed = 2.0f;
	float m_mouseSensitivity = 0.1f;
	float m_xVel{};
	float m_yVel{};
	bool m_mouseMotion = false;
};
} // namespace FG24
