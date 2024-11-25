#pragma once
#include <cstdint>

namespace FG24 {
namespace Shader {
  // Returns 0 on failure
  std::uint32_t CompileShader(const char* vertPath, const char* fragPath);
} // namespace Shader
} // namespace FG24
