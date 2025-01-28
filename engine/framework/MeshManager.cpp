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
std::vector<std::string_view> names;

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

		// TODO: Filepath class should handle file endings 
	    // Attempt to load .mdl
		std::string mdlPath = "../../assets/mesh/";
		mdlPath += meshSource;
		mdlPath += ".mdl";
		// Attempt to load mdl
		VertexData mdlData;
		auto mdlEc = ObjToMdl::LoadMdlToVertexData(mdlPath.c_str(), mdlData);;
		if (mdlEc == ObjToMdl::ErrorCode::Ok) {
			// File exists and was loaded successfully. Use that as the result
			Mesh* mesh = new Mesh(meshSource);
			mesh->InitBuffers(mdlData.m_data, mdlData.m_numVertexData, mdlData.m_numVertices);
			meshMap.insert({ meshSource, mesh });
			names.push_back(meshSource);
			result = mesh;
		} else if (mdlEc == ObjToMdl::ErrorCode::NoFile) {
			// Attempt to load obj
			std::string path = "../../assets/mesh/";
			path += meshSource;
			path += ".obj";
			VertexData data;
			auto ec = ObjToMdl::LoadObjToVertexData(path.c_str(), data);
			// TODO: Handle error
			assert(ec == ObjToMdl::ErrorCode::Ok);

			// Convert to .mdl
			// TODO: Fix! Bad practice of hiding functionallity in GetMesh function
			// This should probably be handled through editor UI
			ObjToMdl::Serialize(meshSource, data);

			Mesh* mesh = new Mesh(meshSource);
			mesh->InitBuffers(data.m_data, data.m_numVertexData, data.m_numVertices);
			meshMap.insert({ meshSource, mesh });
			names.push_back(meshSource);
			result = mesh;
		} else {
			// Error code bad
			std::fprintf(stderr, "Error:MeshManager failed to provide a new mesh!\n");
		}
	}

	return result;
}

const std::vector<std::string_view>& GetNames(){
	return names;
}

} // namespace MeshManager
} // namespace FG24
