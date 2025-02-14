#include <glad/gl.h>
#include <SDL2/SDL_opengl.h>
#include <cassert>
#include "Entity.hpp"
#include "renderer/Mesh.hpp"
#include "renderer/MeshBlend.hpp"
#include <cstdint>
#include <cstdio>
#include "framework/MeshManager.hpp"
#include "Globals.hpp"

namespace FG24 {
Entity::Entity(
	const Mesh* mesh,
	std::uint32_t shaderId,
	std::uint32_t textureId,
	const char* name)
	: m_mesh(mesh),
	  m_shaderId(shaderId),
	  m_textureId(textureId),
	  m_name(name) {
  
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

const std::string& Entity::GetName() const {
	return m_name;
}

void Entity::SetName(const char* name) {
	m_name = name;
}

const Mesh* Entity::GetMesh() const {
	return m_mesh;
}

void Entity::SetMesh(const Mesh* mesh) {
	m_mesh = mesh;
}

void Entity::Draw() const {
    auto shader = m_shaderId;
	assert(shader != 0);
	if (g_useFlatShader) {
	    if (m_mesh->IsBlend()) {
			shader = g_flatBlendShader;
		} else {
			shader = g_flatShader;
		}
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_textureId); 
		glUniform1i(glGetUniformLocation(shader, "albedoMap"), 0);
		
		m_mesh->Draw(shader, m_drawAsWireframe);
	} else {
		if (m_mesh->IsBlend()) {
			shader = g_blendShader;
		}
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_textureId); 
		glUniform1i(glGetUniformLocation(shader, "albedoMap"), 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_textureSpecularId);
		glUniform1i(glGetUniformLocation(shader, "specularMap"), 1);

		m_mesh->Draw(shader, m_drawAsWireframe);
	}
}

void Entity::DrawLightPass(std::uint32_t lightPassShaderId) const {
	m_mesh->Draw(lightPassShaderId, false);
}

bool Entity::WriteTo(FILE* file) const {
	// Lets just write the mesh name for now but it bloats the file size
    std::size_t n = 0;
	std::uint32_t nameLength = m_name.length() + 1; // +1 for '\0'
	assert(nameLength <= 32);
	n += std::fwrite(&nameLength, sizeof(std::uint32_t), 1, file);
	n += std::fwrite(m_name.c_str(), sizeof(char), nameLength, file);
	std::uint32_t mNameLength = m_mesh->GetName().length() + 1;
	assert(mNameLength <= 32);
	n += std::fwrite(&mNameLength, sizeof(std::uint32_t), 1, file);
	n += std::fwrite(m_mesh->GetName().data(), sizeof(char), mNameLength, file);
	n += std::fwrite(&m_shaderId, sizeof(std::uint32_t), 1, file);
	// TODO: Add specular mapping!
	n += std::fwrite(&m_textureId, sizeof(std::uint32_t), 1, file);
	// 4 for nameLength, mNameLength, shaderId, textureId
	std::size_t expected = 4 + nameLength + mNameLength;

	if (n != expected) {
		std::fprintf(stderr, "Error: Entity did not write expected amount\n");
		return false;
	}

	return m_transform.WriteTo(file);

}

bool Entity::ReadFrom(FILE* file) {
	std::size_t n = 0;
	std::uint32_t nameLength = 0;
	char name[32];
	std::uint32_t meshNameLength = 0;
	char meshName[32];
	n += std::fread(&nameLength, sizeof(std::uint32_t), 1, file);
	n += std::fread(&name[0], sizeof(char), nameLength, file);
	n += std::fread(&meshNameLength, sizeof(std::uint32_t), 1, file);
	n += std::fread(&meshName[0], sizeof(char), meshNameLength, file);

	std::uint32_t shaderId = 0;
	n += std::fread(&shaderId, sizeof(std::uint32_t), 1, file);
	std::uint32_t textureId = 0;
	n += std::fread(&textureId, sizeof(std::uint32_t), 1, file);
	
	std::size_t expected = 4 + nameLength + meshNameLength;
	if (n != expected) {
		std::fprintf(stderr, "Error: Entity did not read expected amount\n");
		return false;
	}

	// Only apply if everything is good!
	if (m_transform.ReadFrom(file) == false) {
		return false;
	}

	// Apply everything only when all components are also good
	m_name = name;
	m_mesh = MeshManager::GetMesh(meshName);
	m_shaderId = shaderId;
	m_textureId = textureId;

	return true;
}

} // namespace FG24
