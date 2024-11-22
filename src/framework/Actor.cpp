#include <glad/gl.h>
#include <SDL2/SDL_opengl.h>
#include <cassert>
#include "Actor.hpp"
#include "renderer/Mesh.hpp"
#include "renderer/Material.hpp"

namespace FG24 {
Actor::Actor(Material* material, Mesh* mesh) {

}

void Actor::Draw() {
	assert(mesh->GetVAO());
	assert(mesh->GetVBO());

	// glUseProgram(shaderProgram);
	glBindVertexArray(mesh->GetVAO());
	if (mesh->GetEBO() > 0) {
		// Draw the square. It has an EBO and 6 verticies
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);  
	} else if (mesh->GetEBO() == 0) {
		// Draw the triangle. it has no EBO and only 3 verticies
		glDrawArrays(GL_TRIANGLES, 0, 3);
	} // else if (material->HasTexture())
	
	glBindVertexArray(0); // Unbind
}

} // namespace FG24
