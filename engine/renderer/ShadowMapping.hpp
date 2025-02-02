#pragma once
#include <cstdint>
#include <vector>
#include <glad/gl.h>
#include <SDL2/SDL_opengl.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace FG24 {
class Entity;
class Camera;
class ShadowMapping {
public:
	bool Init(float resolution);
    void Render(const std::vector<Entity*>& entities);
    void CalcShadowMappingMatrix();
private:
    float m_resolution;
	glm::mat4x4 m_shadowMappingMatrix;
    Camera* m_cam = nullptr;
	std::uint32_t m_lightPassShaderId;
  
	GLuint m_framebufferId;
	GLuint m_textureId; // Shadow map texture id. Where we store depth render data
    std::uint32_t m_lightPovCameraId;
};
} // namespace FG24
