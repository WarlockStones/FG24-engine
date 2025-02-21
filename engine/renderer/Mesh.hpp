#pragma once
#include <variant>
#include <string_view>
#include "renderer/MeshSingle.hpp"
#include "renderer/MeshBlend.hpp"
#include "renderer/VertexData.hpp"

namespace FG24 {
class Mesh {
public:
	Mesh() = default;
	~Mesh() = default;
	void InitSingle(std::string_view name, const VertexData& vertexData);
	void InitBlend(
		std::string_view name,
		const VertexData& data1,
		const VertexData& data2);
	std::string_view GetName() const;
	void SetName(std::string_view name);
	void Draw(std::uint32_t shaderId, bool asWireframe) const;
	float* GetBlendPointer(); // Returns nullptr if not a MeshBlend
	bool IsBlend() const;
private:
	std::variant<MeshSingle, MeshBlend> m_mesh;
};
} // namespace FG24
