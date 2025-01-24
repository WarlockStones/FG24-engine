#pragma once
#include <cstddef>
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
	

} // namespace ObjToMdl
} // namespace FG24