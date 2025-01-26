#pragma once
#include <glm/vec3.hpp>

namespace FG24 {
class Camera;
namespace CameraManager {
	Camera* GetActiveCamera();
    void SetActiveCamera(std::uint32_t id);
	Camera& CreateCamera(glm::vec3 position, float yaw, float pitch);
    void RemoveCamera(Camera* camera);
} // namespace CameraManager
} // namespace FG24
