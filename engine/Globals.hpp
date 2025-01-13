#pragma once
#include <cstdint>

namespace FG24 {
class Entity;
class Camera;

// just temporary thing
extern bool g_runGameLoop;
extern double g_deltaTime;

extern int g_windowWidth;
extern int g_windowHeight;

extern std::uint32_t	g_shader;
extern std::uint32_t	g_arcadeTexture;
extern Entity*			g_flag;
extern Camera*			g_camera;

// Inputs
extern float g_yvel;
extern float g_xvel;
extern bool g_action1;
// Inputs - Mouse
// Used to only update camera lookDir when there was a mouse movement
extern bool g_mouseMotion;  // TODO: Fix this messy programming
extern float g_xRel;
extern float g_yRel;
}

