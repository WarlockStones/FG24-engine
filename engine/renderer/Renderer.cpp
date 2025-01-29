#include "Renderer.hpp"
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SDL2/SDL.h>
#include <vector>
#include <cstdio>
#include "Globals.hpp"
#include "framework/Entity.hpp"
#include "Shader.hpp"
#include "framework/Camera.hpp"
#include "framework/CameraManager.hpp"
#include "framework/Lighting.hpp"
#include "renderer/Mesh.hpp"

#include "framework/MeshManager.hpp"

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"

namespace FG24 {
bool Renderer::Init() {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::fprintf(stderr, "Error: Renderer failed to initialize SDL!\n");
		std::fprintf(stderr, "%s\n", SDL_GetError());
		return false;
	}

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	uint32_t windowFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_MOUSE_GRABBED;

	m_window = SDL_CreateWindow("FG24 Engine", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, g_windowWidth, g_windowHeight, windowFlags);

	m_context = SDL_GL_CreateContext(m_window);

	if (!gladLoadGL(GLADloadfunc(SDL_GL_GetProcAddress))) {
		std::fprintf(stderr, "Error: Renderer failed to initialize GLAD!\n");
		return false;
	}

	// Hide cursor and constrain it to the window
	SDL_SetRelativeMouseMode(SDL_TRUE); 

	// 0 = none, 1 = vsync, -1 = adaptive vsync
	if (SDL_GL_SetSwapInterval(0) != 0) {
		std::fprintf(stderr, "Error: Renderer failed to set SDL swap interval!\n");
		std::fprintf(stderr, "%s\n", SDL_GetError());
		return false;
	}

	glEnable(GL_DEPTH_TEST);

	SetProjectionMatrix(45.0f, g_windowWidth, g_windowHeight);

	return true;
}

void Renderer::Draw(const std::vector<Entity*>& entities) {
	glClearColor(0.21f, 0.21f, 0.21f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	const Camera* camera = CameraManager::GetActiveCamera();
	if (camera == nullptr) {
		// TODO: Draw error message in 3D view
		std::printf("No active camera...\n");
		SDL_GL_SwapWindow(m_window);
		return;
	}

	if (camera->projection == ProjectionType::Perspective) {
		SetProjectionMatrix(camera->m_fov, g_windowWidth, g_windowHeight);
	} else {
		// Orthographic camera defines a cube like frustum in world space coordinates
		m_projection = glm::ortho(
			-10.0f, 10.0f, -10.0f, 10.0f, // left, right, bottom, top
			0.0f, 100.0f);  // near clip, far clip
	}

	// Space / Coordinate systems
	// Local/object space > World space > View/eye space > Clip space > Screen space.

	// TODO: Support more than one shaderID
	Shader::Use(g_shader);
	Shader::SetMat4(g_shader, "view", camera->GetViewMatrix());
	Shader::SetMat4(g_shader, "projection", m_projection);
	Shader::SetVec3(g_shader, "cameraPosition", camera->GetPosition());

	int lightType[Lighting::maxLights];
	float diffuse[Lighting::maxLights * 4];
	float specular[Lighting::maxLights * 4];
	float position[Lighting::maxLights * 3];
	float rotation[Lighting::maxLights * 3];
	float attenuation[Lighting::maxLights * 3];
	float cutoff[Lighting::maxLights];
	// attenuation but add that to Light struct
	int activeLights = 0;
	for (const auto& l : Lighting::GetLights()) {
		if (l->enabled) {
			lightType[activeLights] = l->m_type;
			diffuse [activeLights * 4 + 0] = l->m_diffuse.r;
			diffuse [activeLights * 4 + 1] = l->m_diffuse.g;
			diffuse [activeLights * 4 + 2] = l->m_diffuse.b;
			diffuse [activeLights * 4 + 3] = l->m_diffuse.a;

			specular[activeLights * 4 + 0] = l->m_specular.r;
			specular[activeLights * 4 + 1] = l->m_specular.g;
			specular[activeLights * 4 + 2] = l->m_specular.b;
			specular[activeLights * 4 + 3] = l->m_specular.a;

			attenuation[activeLights * 3 + 0] = l->m_attenuation.x;
			attenuation[activeLights * 3 + 1] = l->m_attenuation.y;
			attenuation[activeLights * 3 + 2] = l->m_attenuation.z;

			position[activeLights * 3 + 0] = l->m_position.x;
			position[activeLights * 3 + 1] = l->m_position.y;
			position[activeLights * 3 + 2] = l->m_position.z;

			rotation[activeLights * 3 + 0] = l->m_rotation.x;
			rotation[activeLights * 3 + 1] = l->m_rotation.y;
			rotation[activeLights * 3 + 2] = l->m_rotation.z;

			static auto co = glm::cos(glm::radians(12.5f));
			cutoff[activeLights] = co;
			
			++activeLights;
			// Draw render light default debug mesh
			glm::mat4 model = glm::mat4(1);
			model = glm::translate(model, l->m_position);
			model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
			// TODO: Light's mesh should use default unlit shader
			Shader::SetMat4(g_shader, "model", model);
			// TODO: Add diferent mesh for different light type
			MeshManager::GetMesh("cube")->Draw(g_shader);
		}
	}

	// Todo make Shader:: functions to set arrays
	glUniform1i(glGetUniformLocation(g_shader, "activeLights"), activeLights);
	glUniform4fv(glGetUniformLocation(g_shader, "lightDiffuse"), activeLights, diffuse);
	glUniform4fv(glGetUniformLocation(g_shader, "lightSpecular"), activeLights, specular);
	glUniform3fv(glGetUniformLocation(g_shader, "lightPosition"), activeLights, position);
	glUniform3fv(glGetUniformLocation(g_shader, "lightAttenuation"), activeLights, attenuation);
	glUniform3fv(glGetUniformLocation(g_shader, "lightRotation"), activeLights, rotation);
	glUniform1fv(glGetUniformLocation(g_shader, "lightCutoff"), activeLights, cutoff);
	glUniform1iv(glGetUniformLocation(g_shader, "lightType"), activeLights, lightType);
	Shader::SetVec4(g_shader, "lightAmbient", Lighting::ambient);


	for(const Entity* e : entities) {
		glm::mat4 tr =  glm::mat4(1);
		glm::mat4 rot = glm::mat4(1);
		glm::mat4 scl = glm::mat4(1);
		tr = glm::translate(tr, e->m_transform.GetLocation());
		rot = e->m_transform.GetRotationMatrix();
		scl = glm::scale(scl, e->m_transform.GetScale());
		glm::mat4 model = tr * rot * scl;
		Shader::SetMat4(g_shader, "model", model);

		// Telling each samper to which texture unit it belongs to only needs to be done once
		// Shader::SetInt(g_shader, "tex", 0);

		glUniform1i(glGetUniformLocation(g_shader, "tex"), 0);
		e->Draw();
	}
	
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	SDL_GL_SwapWindow(m_window);
}

void Renderer::SetProjectionMatrix(float fov,
	std::uint32_t windowWidth,
	std::uint32_t windowHeight,
	float nearClipPlane,
	float farClipPlane) 
{
	// Don't run perspective calculation in update loop.
	// Projection matrix only needs to calculate on FOV, aspect ratio, or clip-plane changes
	m_projection = glm::perspective(
		glm::radians(fov),
		static_cast<float>(g_windowWidth) / static_cast<float>(g_windowHeight),
		0.1f, 100.0f);

	// Can be changed to orthographic view if such camera is desired
}

Renderer::~Renderer() {
	if (m_window) {
		SDL_DestroyWindow(m_window);
	}
	if (m_context) {
		SDL_GL_DeleteContext(m_context);
	}
	SDL_Quit();

	std::printf("Renderer destructor done\n");
}
} // namespace FG24
