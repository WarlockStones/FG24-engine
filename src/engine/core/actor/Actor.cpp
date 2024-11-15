#include "Actor.hpp"

namespace FG24 {
Actor::Actor(Mesh* mesh, Shader* shader, Transform transform)
	:	mesh(mesh), shader(shader), transform(transform) {
	
}
} // namespace FG24