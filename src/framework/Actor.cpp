#include <glad/gl.h>
#include <SDL2/SDL_opengl.h>
#include <cassert>
#include "Actor.hpp"
#include "renderer/Mesh.hpp"
#include "renderer/Material.hpp"

#include <cstdio>

namespace FG24 {
Actor::Actor(Mesh* mesh, Material* material) :
	mesh(mesh), material(material) {

}

void Actor::Draw() {
	assert(mesh->GetVAO());
	assert(mesh->GetVBO());

	glUseProgram(material->GetShader());

	glBindVertexArray(mesh->GetVAO());
	if (material->GetTexture() != 0) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, material->GetTexture());
	}
	else if (mesh->GetEBO() > 0) {
		// Draw the square. It has an EBO and 6 vertices
		glDrawArrays(GL_TRIANGLES, 0, 3);
	} else if (mesh->GetEBO() == 0) {
		// Draw the triangle. it has no EBO and only 3 vertices
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);  
	}
	
	glBindVertexArray(0); // Unbind
}

} // namespace FG24
