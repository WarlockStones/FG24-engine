#pragma once
#include <string_view>
#include <vector>

namespace FG24 {
class Mesh;
namespace MeshManager {
	// Load meshes, cache and re-use them
	Mesh* GetMesh(std::string_view meshSource);
	const std::vector<std::string_view>& GetNames();

} // namespace MeshManager
} // namespace FG24