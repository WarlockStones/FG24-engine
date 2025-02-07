#pragma once
#include <string_view>
#include <vector>
#include "renderer/VertexData.hpp"

namespace FG24 {
class Mesh;
namespace MeshManager {
	// Load meshes, cache and re-use them
	Mesh* GetMesh(std::string_view meshSource);
    VertexData LoadVertexData(std::string_view meshSource);
    Mesh* AddMesh(std::string_view meshSource, VertexData& data);
	const std::vector<std::string_view>& GetNames();

} // namespace MeshManager
} // namespace FG24
