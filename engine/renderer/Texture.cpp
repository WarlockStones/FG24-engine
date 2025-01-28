#include "Texture.hpp"
#include "utils/File.hpp"
#include <cstdio>
#include <string_view>
#include <vector>
#include <glad/gl.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_opengl.h>

namespace FG24 {
namespace Texture {
std::vector<std::string_view> names;

std::uint32_t LoadFromFile(const char* path, const char* displayName) {
	SDL_Surface* surface = IMG_Load(path);

	if (surface == nullptr) {
		std::fprintf(stderr, "Error: Texture failed to load m_textureID!\n");
		return false;
	}

	std::uint32_t textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	int mode = GL_RGB;
	if(surface->format->BytesPerPixel == 4) {
		mode = GL_RGBA;
	}

	glTexImage2D(
		GL_TEXTURE_2D,
		0, // Desired mipmap level if generating manually
		mode, // Color mode. RGB or RGBA
		surface->w, surface->h, // Image width and height
		0, // Legacy stuff no longer in use
		mode, // Color format of the source image
		GL_UNSIGNED_BYTE, // Datatype of source image
		surface->pixels); // The data of source image

	// Set wrapping/filtering options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenerateMipmap(GL_TEXTURE_2D); // Automatically generate mipmap textures

	SDL_FreeSurface(surface);

	if (textureID != 0) {
		names.push_back(displayName);
	}

	return textureID;
}

std::string_view GetName(std::int32_t id) {
	if (id == 0 || names.empty()) {
		// OpenGL id 0 and means no texture
		return "None";
	}
	if (id < static_cast<std::int32_t>(names.size())) {
		return names[id]; 
	}
	return "SOMETHING WENT WRONG"; // Attempting to access out of bounds
}

const std::vector<std::string_view>& GetNames() {
	return names;
}



} // namespace Texture
} // namespace FG24
