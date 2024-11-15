#pragma once
#include <glm/ext/vector_float3.hpp>
#include "../Transform.hpp"
namespace FG24 {
	class Mesh;
	// class Texture;
	class Shader;

class Actor {
public:
	Actor(Mesh* mesh, Shader* shader, Transform transform);
	Transform transform{};
private:
	Mesh* mesh;
	Shader* shader;
};

}