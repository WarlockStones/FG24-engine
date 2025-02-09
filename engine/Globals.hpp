#pragma once
#include <cstdint>
#include <glm/vec3.hpp>
#include <vector>
#include "renderer/Texture.hpp"

namespace FG24 {
class Entity;
class Camera;
class Mesh;
class Light;

extern bool g_runGameLoop;

// Configs
extern int g_windowWidth;
extern int g_windowHeight;
constexpr glm::vec3 g_worldUp = glm::vec3(0, 1, 0);
constexpr glm::vec3 g_worldForward = glm::vec3(0, 0, -1);

extern bool g_drawLightsAsWireframe;

// Assets / Entities
extern std::uint32_t				g_shader;
extern std::uint32_t		        g_blendShader;
extern std::uint32_t				g_arcadeTexture;
extern Entity*						g_entity1;
extern Entity*						g_entity2;
extern Camera*						g_camera;
extern Light*						g_light1;
extern Light*						g_light2;
extern Light*						g_light3;
extern Light*						g_lightDir;

// Inputs
extern float g_yvel;
extern float g_xvel;
extern bool g_action1;
extern bool g_action2;
extern bool g_action3;
extern bool g_action4;
extern float g_xRelativeMouseMotion;
extern float g_yRelativeMouseMotion;
extern bool g_mouseIsGrabbed;
} // namespace FG24
