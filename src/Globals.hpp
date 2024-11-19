#pragma once
#include <cstdint>

namespace FG24 {
class Triangle;

// just temporary thing
extern bool g_runGameLoop;
extern double g_deltaTime;

extern int g_windowWidth;
extern int g_windowHeight;

extern std::uint32_t g_triangleShaderProgram;

extern Triangle* g_Triangle;

// Inputs
extern int g_yvel;
extern int g_xvel;
}

