#pragma once
#include <cstdint>
#include "renderer/Triangle.hpp"

namespace FG24 {
// Just temporary thing
bool g_runGameLoop = false;
double g_deltaTime = 0;

int g_windowWidth = 800;
int g_windowHeight = 600;

std::uint32_t g_triangleShaderProgram{};

Triangle* g_Triangle{};

// Key inputs
int g_yvel = 0;
int g_xvel = 0;
}

