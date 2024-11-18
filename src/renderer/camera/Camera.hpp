#pragma once
#include <glm/fwd.hpp>
#include "../../engine/core/Transform.hpp"
#include <glm/glm.hpp>
#include <iostream>

namespace FG24 {

class Camera {
public:
  // Camera properties
  glm::vec3 position;
  glm::vec3 front;
  glm::vec3 up;

  float pitch {0};
  float yaw {0};
  
  float movementSpeed{};
  float mouseSensitivity{};
  float fov{45.0f};

  float xvel{};
  float yvel{};
  
  bool mouseMotion = false;

  Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, -5.0f),
	 glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f),
	 glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
	 float fov = 45.0f,
	 float movementSpeed = 2.0f,
	 float mouseSensitivity = 0.1f);
  void UpdateCameraVectors();
  void ProcessInput();
  glm::mat4 GetViewMatrix();

  void PrintFPS(); // Just a quick dirty thing

private:
  int mouseXPos{};
  int mouseYPos{};
  int mouseLastX{};
  int mouseLastY{};

  bool firstMouseCheck;
};

} // namespace FG24
