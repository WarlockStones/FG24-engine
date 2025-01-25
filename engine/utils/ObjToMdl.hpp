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
	LoadObjFailed,
	SaveObjFailed,
	LoadMdlFailed
};

ErrorCode LoadObjToVertexData(Filepath filepath, VertexData& vertexDataOut);
ErrorCode LoadMdlToVertexData(Filepath filepath, VertexData& vertexDataOut);

ErrorCode Serialize(std::string_view name, const VertexData& data);
} // namespace ObjToMdl
} // namespace FG24
