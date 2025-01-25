#include "Filepath.hpp"
#include <cstddef>
#include <cstdio>
#include <cstdio>
#include <cstring>

void FormatPath(char* s, std::size_t size) {
#ifdef _WIN32
	for (size_t i = 0; i < size && s[i] != '\0'; i++) {
		if (s[i] == '/') {
			s[i] = '\\';
		}
	}
#endif
}

Filepath::Filepath(const char* filepath) {
	std::size_t pathlen = std::strlen(filepath);
	constexpr int maxLength = 254;
	if (pathlen > maxLength) { // 
		// TODO: Do proper error handling
		std::fprintf(stderr, "ERROR: Path to file is too long!\n%s\n", path);
		// I should probably terminate or something :S
	}
	std::memcpy(path, filepath, maxLength);
	FormatPath(path, sizeof(path));
}

const char* Filepath::ToString() {
	return &path[0];
}
