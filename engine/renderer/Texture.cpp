#include "Texture.hpp"
#include "utils/File.hpp"
#include <cstdio>
#include <cstring>
#include <string_view>
#include <vector>
#include <unordered_map>
#include <glad/gl.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_opengl.h>

namespace FG24 {
namespace Texture {
// Because of shadowmap texture I can not just map directly to texture id
// Textures that can be assigned to mesh (i.e. does not include shadowmap)
std::unordered_map<std::string_view, GLuint> nameToId; 
std::unordered_map<GLuint, std::string_view> idToName;
std::vector<std::string_view> names; // To easy access for UI


std::uint32_t LoadFromFile(const char* path, const char* displayName) {
	SDL_Surface* surface = IMG_Load(path);

	if (surface == nullptr) {
		std::fprintf(stderr, "Error: Texture failed to load!\n\t%s\n", path);
		return false;
	}

	// Flip SDL surface
	SDL_LockSurface(surface);
	int pitch = surface->pitch; // Distance in bytes between rows of pixels
	std::uint8_t* temp = new std::uint8_t[pitch];
	std::uint8_t* pixels = static_cast<std::uint8_t*>(surface->pixels);

	for (int i = 0; i < surface->h / 2; ++i) {
		std::uint8_t* r1 = pixels + i * pitch; // Access row i
		// Use height of surface to access opposing row on other side
		std::uint8_t* r2 = pixels + (surface->h - i - 1) * pitch;

		// Swap rows
		std::memcpy(temp, r1, pitch);
		std::memcpy(r1, r2, pitch);
		std::memcpy(r2, temp, pitch);
	}
	delete[] temp;
	SDL_UnlockSurface(surface); 

	// Generate OpenGL Texture
	std::uint32_t textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);

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

	if (textureId != 0) {
		nameToId.insert({displayName, textureId});
		idToName.insert({textureId, displayName});
		names.push_back(displayName);
	}

	return textureId;
}

std::uint32_t GetId(std::string_view name) { 
	if (auto search = nameToId.find(name); search != nameToId.end()) {
		return search->second;
	} 

	std::printf( "Texture:GetId: Could not find texture of name: %s\n", name.data());
	return 0;
}

std::string_view GetName(std::uint32_t id) {
	if (auto search = idToName.find(id); search != idToName.end()) {
		return search->second;
	}

	// std::printf( "Texture:GetName: Could not find texture with id: %u\n", id);

	return "None";
}

const std::vector<std::string_view>& GetNames() {
	return names;
}

// This can be improved in OpenGL 3.2 using "Texture Sampler Objects"
void UpdateMipMapSettings(bool linear) {
	// Texture test, this should probably not be on tick
	for (int i = 0; i < Texture::GetNames().size(); ++i) {
		glBindTexture(GL_TEXTURE_2D, i + 1); // OpenGL id starts at 1
		if (linear) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}  else {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		}
	}
}

} // namespace Texture
} // namespace FG24
