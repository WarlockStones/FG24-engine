#include "Mesh.hpp"
#include <string_view>
#include <variant>
#include "renderer/MeshSingle.hpp"
#include "renderer/MeshBlend.hpp"
#include "renderer/VertexData.hpp"

namespace FG24 {
// Init a single mesh
void Mesh::InitSingle(std::string_view name, const VertexData& vertexData) {
	// Init std::variant as MeshSingle
	m_mesh.emplace<MeshSingle>();
	std::get_if<MeshSingle>(&m_mesh)->Init(name, vertexData);
}

// Init a mesh that has multiple buffer for blend shapes
void Mesh::InitBlend(
	std::string_view name, 
	const VertexData& data1,
	const VertexData& data2) 
{
	m_mesh.emplace<MeshBlend>();
	std::get_if<MeshBlend>(&m_mesh)->Init(name, data1, data2);
}

std::string_view Mesh::GetName() const {
	if (const MeshSingle* p = std::get_if<MeshSingle>(&m_mesh)) {
		return p->m_name;
	} else if (const MeshBlend* p = std::get_if<MeshBlend>(&m_mesh)) {
		return p->m_name;
	} else {
		std::fprintf(
			stderr,
			"Error: Mesh::GetName. Could not get value of std::variant!\n");
	}

	return "ERROR IN Mesh::GetName()!";
}

void Mesh::SetName(std::string_view name) {
	if (MeshSingle* p = std::get_if<MeshSingle>(&m_mesh)) {
		p->m_name = name;
	} else if (MeshBlend* p = std::get_if<MeshBlend>(&m_mesh)) {
		p->m_name = name;
	} else {
		std::fprintf(
			stderr,
			"Error: Mesh::SetName. Could not value of std::variant!\n");
	}
}

void Mesh::Draw(std::uint32_t shaderId, bool asWireframe) const {
	if (const MeshSingle* p = std::get_if<MeshSingle>(&m_mesh)) {
		p->Draw(shaderId, asWireframe);
	} else if (const MeshBlend* p = std::get_if<MeshBlend>(&m_mesh)) {
		p->Draw(asWireframe);
	} else {
		std::fprintf(
			stderr,
			"Error: Mesh::SetName. Could not value of std::variant!\n");
	}
}

float* Mesh::GetBlendPointer() {
	if (MeshBlend* p = std::get_if<MeshBlend>(&m_mesh)) {
		return &p->m_blendAmount;
	} 

	return nullptr;
}

bool Mesh::IsBlend() const {
	if (const MeshBlend* p = std::get_if<MeshBlend>(&m_mesh)) {
	    return true;
	}

	return false;
}

} // namespace FG24
