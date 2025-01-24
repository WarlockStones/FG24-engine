#include "MeshManager.hpp"
#include <unordered_map>
#include <cassert>
#include "renderer/Mesh.hpp"
#include "utils/ObjToMdl.hpp"
#include <string>

namespace FG24 {
namespace MeshManager {
// Map can be cleared by for each loop and freeing memory
std::unordered_map<std::string_view, Mesh*> meshMap;

// If Mesh is not stored, attempt to load it into memory
[[nodiscard]]
Mesh* GetMesh(std::string_view meshSource) {
	Mesh* result = nullptr;
	if (auto search = meshMap.find(meshSource); search != meshMap.end()) {
		result = search->second;
	} else {
		// Attempt to load mesh
		//   Return newly loaded mesh on success
		//   Return default mesh on failure

		// TODO:
		// First attemp to load .mdl
		// Look for .obj
		// Convert .obj to .mdl
		// Load .mdl


		// For now just load OBJ to test
		std::string path = "../../assets/mesh/";
		path += meshSource;
		path += ".obj";
		VertexData data;
		auto ec = ObjToMdl::LoadObjToVertexData(path.c_str(), data);
		// TODO: Handle error
		assert(ec == ObjToMdl::ErrorCode::Ok);

		Mesh* mesh = new Mesh;
		mesh->InitBuffers(data.m_data, data.m_numVertexData, data.m_numVertices);
		meshMap.insert({ meshSource, mesh });
		result = mesh;
	}

	return result;
}

} // namespace MeshManager
} // namespace FG24