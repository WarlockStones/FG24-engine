#pragma once
#include <cstdint>

namespace FG24 {
class Texture {
public:
	bool LoadFromFile(const char* path);
	std::uint32_t Get() const;
private:
	std::uint32_t textureID{};
};
} // namespace FG24
