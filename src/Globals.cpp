#include <cstdint>
#include "renderer/Triangle.hpp"
#include "renderer/Shader.hpp"
#include "renderer/Mesh.hpp"
#include "framework/Actor.hpp"

namespace FG24 {
// Just temporary thing
bool g_runGameLoop = false;
double g_deltaTime = 0;

int g_windowWidth = 800;
int g_windowHeight = 600;

Actor* g_triangle{};
std::uint32_t		g_texturedShader{};
std::uint32_t		g_arcadeTexture{};
Mesh*				g_triangleMesh{};

// Key inputs
int g_yvel = 0;
int g_xvel = 0;
}

