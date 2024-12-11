#include <glad/gl.h>
#include <SDL2/SDL_opengl.h>
#include <cassert>
#include "Actor.hpp"
#include "renderer/Mesh.hpp"
#include <cstdint>

#include <cstdio>

namespace FG24 {
Actor::Actor(Mesh* mesh, std::uint32_t shader, std::uint32_t texture) :
	mesh(mesh), shader(shader), texture(texture) {
}
void Actor::Draw() {
	// Set shader and character specific things here 

	if (texture != 0) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
	}

	// Use shader and do shader stuff

	mesh->Draw(shader);

	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture
}

constexpr int ActorVersion = 1; // For serialization
bool Actor::WriteTo(FILE* file) const {
	int version = ActorVersion;
	std::fwrite(&version, sizeof(int), 1, file);
	transform.WriteTo(file);

	return true;
}

bool Actor::ReadFrom(FILE* file) {
	int version = 0;
	std::fread(&version, sizeof(int), 1, file);
	transform.ReadFrom(file);

	return true;
}

} // namespace FG24
