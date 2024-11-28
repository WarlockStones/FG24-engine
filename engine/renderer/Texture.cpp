#include "Texture.hpp"
#include "utils/File.hpp"
#include <cstdio>
#include <glad/gl.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_opengl.h>

namespace FG24 {
namespace Texture {
std::uint32_t LoadFromFile(const char* path) {
	SDL_Surface* surface = IMG_Load(path);

	if (surface == nullptr) {
		std::fprintf(stderr, "Error: Texture failed to load texture!\n");
		return 0;
	}

	std::uint32_t textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	int mode = GL_RGB;
	if(surface->format->BytesPerPixel == 4) {
		mode = GL_RGBA;
	}

	glTexImage2D(GL_TEXTURE_2D,
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

	return textureID;
}
} // namesapce Texture
} // namespace FG24
