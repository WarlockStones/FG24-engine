// Temporary things for when developing features of the Renderer.
// These things are here to not clutter the overall intended structure.
#pragma once
#include <cstdint>
namespace FG24 {
class Triangle;

class TempTriangle {
public:
	static void Init();
	static void Draw();
private:
	static std::uint32_t shaderProgram;
	static Triangle* triangle;

	static bool CompileShader();
};
} // namespace FG24

