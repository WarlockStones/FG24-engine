#pragma once
#include <cstdint> 
#include <string_view>
#include <vector>

namespace FG24 {
namespace Texture {
std::uint32_t LoadFromFile(const char* path, const char* displayName);
std::string_view GetName(std::int32_t id);
const std::vector<std::string_view>& GetNames();
} // namespace Texture
} // namespace FG24
