#pragma once
#include <cstdint>
#include <glm/mat4x4.hpp>

namespace FG24 {
namespace Shader {
  // Returns 0 on failure
  std::uint32_t CompileShader(const char* vertPath, const char* fragPath);
  void Use(std::uint32_t shaderID);
  void SetInt  (std::uint32_t shaderID, const char* name, int value);
  void SetFloat(std::uint32_t shaderID, const char* name, float value);
  void SetVec3 (std::uint32_t shaderID, const char* name, glm::vec3 value);
  void SetVec4 (std::uint32_t shaderID, const char* name, glm::vec4 value);
  void SetMat4 (std::uint32_t shaderID, const char* name, glm::mat4 value);
} // namespace Shader
} // namespace FG24
