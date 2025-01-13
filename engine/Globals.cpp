#include <cstdint>
#include "renderer/Shader.hpp"
#include "renderer/Mesh.hpp"
#include "framework/Entity.hpp"

namespace FG24 {
// Just temporary thing
bool g_runGameLoop = false;
double g_deltaTime = 0;

int g_windowWidth = 800;
int g_windowHeight = 600;

std::uint32_t		g_shader{};
std::uint32_t		g_arcadeTexture{};
Entity*				g_flag{};

// Key inputs
int g_yvel = 0;
int g_xvel = 0;
bool g_action1 = false;
}

