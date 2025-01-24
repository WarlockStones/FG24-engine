#pragma once
#include <cstddef>
#include <string_view>
#include "utils/File.hpp"
#include "renderer/VertexData.hpp"

namespace FG24 {
namespace ObjToMdl {
enum class ErrorCode {
	Ok,
	NoFile,
	LoadObjFailed
};

ErrorCode LoadObjToVertexData(Filepath filepath, VertexData& vertexDataOut);

// Write vertex data to a binary .mdl file
constexpr int mdlVersion = 1;
ErrorCode SerializeVertexData(std::string_view name, const VertexData& data);

} // namespace ObjToMdl
} // namespace FG24