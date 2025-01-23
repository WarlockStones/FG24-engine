#include <glad/gl.h>
#include <SDL2/SDL_opengl.h>
#include <cassert>
#include "Entity.hpp"
#include "renderer/Mesh.hpp"
#include <cstdint>
#include <cstdio>

namespace FG24 {
Entity::Entity(const Mesh& mesh, std::uint32_t shaderId, std::uint32_t textureId) :
	m_mesh(mesh), m_shaderId(shaderId), m_textureId(textureId) {
}

Entity::Entity(const Entity& other) 
	: m_mesh(other.m_mesh),
	m_transform(other.m_transform),
	m_shaderId(other.m_shaderId),
	m_textureId(other.m_textureId){

}

Entity& Entity::operator=(Entity&& other) {
	if (&other == this) {
		return *this;
	}

	// Release any resources we may be holding
	// Transfer ownership of pointers

	return *this;
}

void Entity::Draw() const {
	// Set m_shaderID and character specific things here 

	if (m_textureId != 0) {
		glActiveTexture(GL_TEXTURE0);
	}

	// Use m_shaderID and do m_shaderID stuff

	m_mesh.Draw(m_shaderId);

	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture
}

constexpr int EntityVersion = 1; // For serialization
bool Entity::WriteTo(FILE* file) const {
	int version = EntityVersion;
	std::fwrite(&version, sizeof(int), 1, file);
	m_transform.WriteTo(file);

	return true;
}

bool Entity::ReadFrom(FILE* file) {
	int version = 0;
	std::fread(&version, sizeof(int), 1, file);
	m_transform.ReadFrom(file);

	return true;
}

} // namespace FG24
