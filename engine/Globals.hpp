#pragma once
#include <cstdint>

namespace FG24 {
class Entity;
class Mesh;
class MeshData;

// just temporary thing
extern bool g_runGameLoop;
extern double g_deltaTime;

extern int g_windowWidth;
extern int g_windowHeight;

extern Entity*			g_triangle;
extern std::uint32_t	g_texturedShader;
extern std::uint32_t	g_arcadeTexture;
extern Mesh*			g_triangleMesh;
extern Entity*			g_flag;

// Inputs
extern int g_yvel;
extern int g_xvel;
}

