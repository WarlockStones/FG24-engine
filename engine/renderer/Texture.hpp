#pragma once
#include <cstdint> 

namespace FG24 {
class Texture {
public:
	Texture(const char* name);
	bool LoadFromFile(const char* path);
	const char* m_name; // Display name for UI
	std::uint32_t m_id; // id from OpenGL
};
} // namespace FG24
