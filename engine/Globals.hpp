#pragma once
#include <cstdint>
#include <glm/vec3.hpp>

namespace FG24 {
class Entity;
class Camera;

extern bool g_runGameLoop;

// Configs
extern int g_windowWidth;
extern int g_windowHeight;
constexpr glm::vec3 g_worldUp = glm::vec3(0, 1, 0);
constexpr glm::vec3 g_worldForward = glm::vec3(0, 0, -1);

// Assets / Entities
extern std::uint32_t	g_shader;
extern std::uint32_t	g_arcadeTexture;
extern Entity*			g_flag;
extern Camera*			g_camera;
extern std::uint32_t	g_light1;
extern std::uint32_t	g_light2;

// Inputs
extern float g_yvel;
extern float g_xvel;
extern bool g_action1;
extern bool g_action2;
extern bool g_action3;
extern bool g_action4;
extern float g_xRelativeMouseMotion;
extern float g_yRelativeMouseMotion;
} // namespace FG24
