#pragma once

namespace FG24 {
class Texture {
public:
	// data?
	// Function which loads image from disk using SDL_Image
	Texture(const char* path);
	~Texture();
};
} // namespace FG24