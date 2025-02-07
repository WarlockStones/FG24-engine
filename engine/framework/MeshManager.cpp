#include "MeshManager.hpp"
#include <unordered_map>
#include <cassert>
#include <string>
#include "renderer/Mesh.hpp"
#include "renderer/VertexData.hpp"
#include "utils/ObjToMdl.hpp"

namespace FG24 {
namespace MeshManager {
// Map can be cleared by for each loop and freeing memory
std::unordered_map<std::string_view, Mesh*> meshMap;
std::vector<std::string_view> names;

// Returns mesh if exists. Otherwise return nullptr
[[nodiscard]]
Mesh* GetMesh(std::string_view meshSource) {
	if (auto search = meshMap.find(meshSource); search != meshMap.end()) {
		return search->second;
	} 

	return nullptr;
}

// Loads vertex data from disk.
VertexData LoadVertexData(std::string_view meshSource) {
		// TODO: Filepath class should handle file endings 
	    // Attempt to load .mdl
		std::string mdlPath = "../../assets/mesh/";
		mdlPath += meshSource;
		mdlPath += ".mdl";
		// Attempt to load mdl
		VertexData data;
		auto ec = ObjToMdl::LoadMdlToVertexData(mdlPath.c_str(), data);
		if (ec == ObjToMdl::ErrorCode::NoFile) {
			// Attempt to load obj
			std::string path = "../../assets/mesh/";
			path += meshSource;
			path += ".obj";
			ec = ObjToMdl::LoadObjToVertexData(path.c_str(), data);
			if (ec == ObjToMdl::ErrorCode::Ok) {
				// Convert to .mdl
				// This should probably be handled through editor UI
				ObjToMdl::Serialize(meshSource, data);
			} else {
				std::fprintf(stderr, "Error:MeshManager failed to provide a new mesh!\n");
				data.m_data = nullptr;
			}
		}

	return data;
}

Mesh* AddMesh(std::string_view meshSource, VertexData& data) {
	Mesh* m = GetMesh(meshSource);
	assert(m == nullptr); // Attempting to add mesh that already exists

	m = new Mesh(meshSource);
	m->InitBuffers(data);

	meshMap.insert({ meshSource, m });
    names.push_back(meshSource);

	return m;
}

const std::vector<std::string_view>& GetNames(){
	return names;
}

} // namespace MeshManager
} // namespace FG24
