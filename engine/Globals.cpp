#include <cstdint>
#include <glm/vec3.hpp>
#include "renderer/Shader.hpp"
#include "renderer/Mesh.hpp"
#include "framework/Entity.hpp"
#include "framework/Camera.hpp"


namespace FG24 {
bool g_runGameLoop = false;

int g_windowWidth = 800;
int g_windowHeight = 600;

std::uint32_t		g_shader{};
std::uint32_t		g_arcadeTexture{};
Entity*				g_entity1{};
Entity*				g_entity2{};
Camera*				g_camera{};
glm::vec3			g_lightPos;
std::uint32_t		g_light = -1;
std::uint32_t		g_light2 = -1;

// Key inputs
float g_yvel = 0;
float g_xvel = 0;
bool g_action1 = false;
bool g_action2 = false;
bool g_action3 = false;
bool g_action4 = false;
// Inputs - Mouse
bool g_mouseMotion{};
float g_xRelativeMouseMotion{};
float g_yRelativeMouseMotion{};
}

