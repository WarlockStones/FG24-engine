#pragma once
#include <glm/vec3.hpp>

namespace FG24 {
class Camera;
namespace CameraManager {
	Camera* GetActiveCamera();
	Camera* GetCamera(std::uint32_t id);
	std::size_t GetNumCameras(); // Get number of initialized cameras
	std::uint32_t* GetIdArray(); // Get packed array of ids for initialized cameras
	void SetActiveCamera(std::uint32_t id);
	Camera& CreateCamera(glm::vec3 position, float yaw, float pitch);
	void RemoveCamera(std::uint32_t id);

} // namespace CameraManager
} // namespace FG24
