#include "CameraManager.hpp"
#include "framework/Camera.hpp"
#include <vector>

namespace FG24 {
namespace CameraManager {
std::vector<Camera> cameras;
Camera* activeCamera = nullptr;

Camera* GetActiveCamera() {
	return activeCamera;
}

void SetActiveCamera(const Camera& camera) {
	activeCamera = const_cast<Camera*>(&camera);
}

[[nodiscard]]
Camera& CreateCamera(glm::vec3 position, float yaw = 0, float pitch = 0) {
    static int id = 0;
	cameras.emplace_back(position, yaw, pitch, id++);
	return cameras.back();
}

void RemoveCamera(int id) {
	for (std::size_t i = 0; i < cameras.size(); ++i) {
		if (cameras[i].m_id == id) {
		    if (activeCamera != nullptr && activeCamera->m_id == id) {
				activeCamera = nullptr;
			}
			cameras.erase(cameras.begin() + i);
		}
	}
}

} // CameraManager
} // FG24
