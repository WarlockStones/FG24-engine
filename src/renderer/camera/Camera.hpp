#pragma once
#include <glm/fwd.hpp>
#include "../../engine/core/Transform.hpp"

namespace FG24 {

class Camera {
public:
    Camera() = default;
	Camera(glm::vec3 position);

	// TODO: Change to Transform
	glm::vec3 position { 0.0f, 0.0f, 0.0f };
	glm::vec3 front {0.0f, 0.0f, -1.0f};
	glm::vec3 up {0.0f, 1.0f, 0.0f};
};

}
