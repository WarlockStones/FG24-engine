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
    Mesh* AddMesh(std::string_view meshSource, const VertexData& data);
    Mesh* AddMesh(
		std::string_view meshSource,
		const VertexData& data1,
		const VertexData& data2);
	const std::vector<std::string_view>& GetNames();

} // namespace MeshManager
} // namespace FG24
