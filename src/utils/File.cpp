#include "File.hpp"
#include <cstdio>
#include <cstdint>
#include <utility>
#include <cstring>
#include <cassert>

namespace FG24 {
static void FormatFilePath(char* s, std::size_t size) {
#ifdef _WIN32
	for (size_t i = 0; i < size; i++) {
		if (s[i] == '/') {
			s[i] = '\\';
		}
	}
#endif
}

const char* LoadTextFile(const char* path) {
	// TODO: Measure lambda vs static function
	auto Check = [path](bool failCondition, const char* functionName) {
		if (failCondition == true) {
			std::perror(functionName);
			std::fprintf(stderr, "Error: LoadTextFile %s failed for path '%s'\n",
				functionName, path);
			return true;
		} else {
			return false;
		}
	};

	std::size_t pathlen = std::strlen(path);
	assert(pathlen < 260);
	// Windows only allows for 260 character paths. This is probably faster than dynamic allocation
	// TODO: Measure stack array vs dynamic array
	char p[260];
	std::memcpy(p, path, sizeof(path));
	FormatFilePath(p, sizeof(path));

	std::FILE* file = std::fopen(path, "r");
	if (Check(file == nullptr, "fopen()")) {
		return nullptr;
	}
		
	int result = std::fseek(file, 0, SEEK_END);
	if (Check(result != 0, "fseek()")) {
		return nullptr;
	}

	std::int32_t fileSize = std::ftell(file);
	if (Check(fileSize == -1L, "ftell()"))  {
		return nullptr;
	}

	result = std::fseek(file, 0, SEEK_SET);
	if (Check(result != 0, "fseek()")) {
		return nullptr;
	}

	// Allocate string of fileSize + 1 for '\0'
	std::size_t textBuffSize = static_cast<std::size_t>(fileSize + 1);
	char* text = new char[textBuffSize]; // TODO: Fix memory leak...

	// TODO: Read count is wrong when not using UNIX line end character
	std::size_t readCount = std::fread(
		static_cast<char*>(text), sizeof(char), static_cast<std::size_t>(fileSize), file);
	if (Check(readCount != static_cast<std::size_t>(fileSize), "fread()")) {

		std::ferror(file);
		std::fprintf(stderr, "Read count: %zd is not file size %I32d\n", readCount, fileSize);
		return nullptr;
	}

	text[fileSize] = '\0';

	std::fclose(file);
	std::printf("Successfully read file of size %I32d\n", fileSize);

	return text;
}
} // FG24 namespace
