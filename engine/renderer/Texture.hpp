#pragma once
#include <cstdint> 
#include <string_view>
#include <vector>

namespace FG24 {
namespace Texture {
std::uint32_t LoadFromFile(const char* path, const char* displayName);
std::uint32_t GetId(std::string_view name); // Get the OpenGL Texture ID
std::string_view GetName(std::uint32_t id); // Get name associated with OpenGL id
const std::vector<std::string_view>& GetNames();
void UpdateMipMapSettings(bool linear);
} // namespace Texture
} // namespace FG24
