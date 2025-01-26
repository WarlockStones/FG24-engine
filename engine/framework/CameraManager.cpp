#include "CameraManager.hpp"
#include "framework/Camera.hpp"
#include <cstdio>
#include <vector>

namespace FG24 {
// Testing useage of not dymanically allocating my objects
namespace CameraManager {
Camera* activeCamera = nullptr;
constexpr std::size_t maxCameras = 24;
std::size_t numRecycledIds = 0;
std::uint32_t recycledIds[maxCameras]; // Packed array of ids that can be reused
std::uint32_t activeIds[maxCameras]; // Packed array of ids
std::size_t numCameras = 0;
Camera cameras[maxCameras]; // Sparse array of cameras
//  (doing it this way with sparse arrays good data base design really comes
//  into play to reduce size/redundancy. A Camera should probably only be a vec3
//  pos, yaw, and pitch. Not the 13 variables it is now.)

Camera* GetActiveCamera() {
	return activeCamera;
}

// Naive algorithm
bool CameraExists(std::uint32_t id) {
	for (size_t i = 0; i < numCameras; ++i) {
		if (activeIds[i] == id) {
			return true;
		}
	}
  
	return false;
}

void SetActiveCamera(std::uint32_t id) {
	// Reference of C not the thing c points to?
	if (CameraExists(id)) {
	    std::printf("Setting active camera to: id: %d, addr: %p\n", id, &cameras[id]);
		activeCamera = &cameras[id];
	}
}

[[nodiscard]]
Camera& CreateCamera(glm::vec3 position, float yaw = 0, float pitch = 0) {
    std::uint32_t id = numCameras;
	if (numRecycledIds > 0) {
		id = recycledIds[0];
		// Move last position to front to keep array packed
		numRecycledIds--;
		recycledIds[0] = recycledIds[numRecycledIds];
    }
	
	cameras[id].Init(position, yaw, pitch, id);

	activeIds[numCameras] = id;
	numCameras++;

	return cameras[id];
}

void RemoveCamera(std::uint32_t id) {
    if (CameraExists(id)) {
		if (activeCamera == &cameras[id]) {
			activeCamera = nullptr;
		}
		
		recycledIds[numRecycledIds++] = id;
		
		for (std::size_t i = 0; i < numCameras; ++i) {
			// Keep array packed by moving last element to this newly freed spot
			if (activeIds[i] == id) {
				activeIds[i] = activeIds[numCameras - 1];
			}
		}

		numCameras--;
	}
}

} // CameraManager
} // FG24
