#include "ShadowMapping.hpp"
#include <glad/gl.h>
#include <SDL2/SDL_opengl.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include "framework/CameraManager.hpp"
#include "framework/Camera.hpp"
#include "renderer/Shader.hpp"
#include "framework/Entity.hpp"

namespace FG24 {

bool ShadowMapping::Init(float resolution) {
    m_resolution = resolution;

	// Create a render buffer for the depth pass
	glGenFramebuffers(1, &m_framebufferId);

	/* // Lecture notes made Renderbuffer. But I could not get it to work
    glGenRenderbuffers(1, &m_renderbufferId);
	
	glBindRenderbuffer(GL_RENDERBUFFER, m_renderbufferId);
	glRenderbufferStorage(
		GL_RENDERBUFFER,
		GL_DEPTH_COMPONENT24,
		resolution, resolution);

	// Attach render buffer to the default frame buffer
	glFramebufferRenderbuffer(
		GL_FRAMEBUFFER,
		GL_DEPTH_ATTACHMENT,
		GL_RENDERBUFFER,
		m_renderbufferId);
	*/

	// Create a texture to store data to pass to the normal render pass
	glGenTextures(1, &m_textureId);
	glBindTexture(GL_TEXTURE_2D, m_textureId);
	glTexImage2D(
		GL_TEXTURE_2D,
		0, // Desired mipmap level. We want to generate it manually later
		GL_DEPTH_COMPONENT, // Internal format. Depth, not RGB
		resolution, resolution, // Width, height
		0, // Legacy stuff, this value must be 0
		GL_DEPTH_COMPONENT, // Format
		GL_FLOAT, // Data type
		nullptr); // Source of data. We will add data to the texture later

	glGenerateMipmap(GL_TEXTURE_2D);
	// Or specify more mipmap settings
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Bind texture to framebuffer so texture can get and store all the ata
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferId); // Bind to our new frameBuffer
	
	glFramebufferTexture2D(
		GL_FRAMEBUFFER,
		GL_DEPTH_ATTACHMENT,
		GL_TEXTURE_2D, 
		m_textureId,
		0); // Mipmap 0

	// Tell OpenGL to not draw this framebuffer
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	glBindFramebuffer(GL_FRAMEBUFFER, 0); // Bind to default (0) to unbind new FBO

	 if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
	   std::fprintf(stderr, "Error: ShadowMapping failed to generate frame buffer!\n");
	   return false;
	 }

	// Camera attributes may need tweaking. Frustum must be good
	// TODO: Get light position should be same as directional light
	m_lightPovCameraId = CameraManager::CreateCamera(glm::vec3(4, 5, 10), -100, -25);
	m_cam = CameraManager::GetCamera(m_lightPovCameraId);
	m_cam->projection = ProjectionType::Orthographic;
	m_cam->UpdateVectors();

	// Shadow Mapping Matrix. MVP transform for the light pass
	glm::mat4 biasMatrix {
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f
	};
	m_shadowMappingMatrix = biasMatrix * m_cam->GetViewMatrix();

	m_lightPassShaderId = Shader::CompileShader(
		"../../assets/shaders/lightpass.vert",
		"../../assets/shaders/lightpass.frag");
	
	// TODO: Check and make sure buffer is good
	return true; 
}

void ShadowMapping::Render(const std::vector<Entity*>& entities) {
	// Render all solid entities that are to cast shadows using the lightpass
	// shader. They will be stored in the texture bound to the Framebuffer

	// ---- Configure for rendering  ----
	glViewport(0,0, m_resolution, m_resolution);
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferId);
	glClear(GL_DEPTH_BUFFER_BIT);
	
	// Lecture note RenderBuffer stuff that I could not get wokring
	// GLenum buffers[1]; // We only need 1 for depth test
	// buffers[0] = GL_DEPTH_ATTACHMENT;
	// Tell opengl to render to this buffer and not to the actual window as by default
	// glDrawBuffers(1, buffers);

	// No need to set or get active camera, just have reference to it
	glm::mat4 view = m_cam->GetViewMatrix();
	static glm::mat4 proj = glm::ortho<float>(-10, 10, -10, 10, -10, 100); // Hard coded
	Shader::Use(m_lightPassShaderId);

	for (const Entity* e : entities) {
		if (e->m_drawAsWireframe == false) {
			glm::mat4 tr =  glm::mat4(1);
			glm::mat4 rot = glm::mat4(1);
			glm::mat4 scl = glm::mat4(1);
			tr = glm::translate(tr, e->m_transform.GetLocation());
			rot = e->m_transform.GetRotationMatrix();
			scl = glm::scale(scl, e->m_transform.GetScale());
			glm::mat4 model = tr * rot * scl;
			glm::mat4 depthMvp = proj * view * model;
			Shader::SetMat4(m_lightPassShaderId, "depthMvp", depthMvp);
			e->DrawLightPass(m_lightPassShaderId);
		}
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

glm::mat4 ShadowMapping::GetDepthBiasMVP(const glm::mat4& model) {
	static glm::mat4 biasMatrix {
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f
	};
	glm::mat4 view = m_cam->GetViewMatrix();
	static glm::mat4 proj = glm::ortho<float>(-10, 10, -10, 10, -10, 100); // Hard coded

	return biasMatrix * proj * view * model;
}
} // namespace FG24
