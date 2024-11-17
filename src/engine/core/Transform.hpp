#pragma once
#include <glm/ext/vector_float3.hpp>

namespace FG24 {

class Transform {
public:
	Transform(glm::vec3 position = { 0,0,0 }, glm::vec3 scale = { 1,1,1 }, glm::vec3 rotation = { 0,0,0 });
		

	glm::vec3 position;
	glm::vec3 scale{};
	glm::vec3 rotation{}; 
};

}
