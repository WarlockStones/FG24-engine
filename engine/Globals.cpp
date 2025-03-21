#include <cstdint>
#include <glm/vec3.hpp>
#include "renderer/Shader.hpp"
#include "renderer/Mesh.hpp"
#include "renderer/Texture.hpp"
#include "framework/Entity.hpp"
#include "framework/Camera.hpp"
#include "framework/Lighting.hpp"


namespace FG24 {
bool g_runGameLoop = false;

int g_windowWidth = 800;
int g_windowHeight = 600;

bool g_drawLightsAsWireframe = true;
bool g_useFlatShader = false;

std::uint32_t			g_shader{};
std::uint32_t			g_blendShader{};
std::uint32_t			g_flatShader{};
std::uint32_t			g_flatBlendShader{};

Entity*					g_entity1{};
Entity*					g_entity2{};
Camera*					g_camera{};
glm::vec3				g_lightPos{};
Light*					g_light1{};
Light*					g_light2{};
Light*					g_light3{};
Light*					g_lightDir{};
Mesh*					g_sphereMesh{};
Mesh*					g_cubeMesh{};
glm::vec3				g_rayOrigin{ 0, -4, 4};
glm::vec3				g_rayDir{ 0, 0, -1};

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
bool g_mouseIsGrabbed = true;
}

