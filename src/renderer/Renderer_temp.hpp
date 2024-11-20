// Temporary things for when developing features of the Renderer.
// These things are here to not clutter the overall intended structure.
#pragma once
#include <cstdint>
namespace FG24 {
class Triangle;
class Square;

enum class RenderType {
	triangle,
	square
};

class TempRenderObject {
public:
	static void Init();
	static void Draw(RenderType typeToDraw);
private:
	static std::uint32_t shaderProgram;
	static Triangle* triangle;
	static Square* square;

	static std::uint32_t CompileSimpleShader();
};

} // namespace FG24

