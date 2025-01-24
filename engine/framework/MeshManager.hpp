#pragma once
#include <string_view>

namespace FG24 {
class Mesh;
namespace MeshManager {
	// Load meshes, cache and re-use them
	Mesh* GetMesh(std::string_view meshSource);

} // namespace MeshManager
} // namespace FG24