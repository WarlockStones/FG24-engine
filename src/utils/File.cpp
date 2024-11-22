#include "File.hpp"
#include <cstdio>
#include <cstdint>
#include <utility>
#include <cstring>
#include <cassert>

namespace FG24 {
static void FormatFilePath(char* s, std::size_t size) {
#ifdef _WIN32
	for (int i = 0; i < size; i++) {
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
	// if (sizeof(path) > 260)
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

	const std::size_t fileSize = std::ftell(file);
	if (Check(fileSize == -1L, "ftell()"))  {
		return nullptr;
	}

	result = std::fseek(file, 0, SEEK_SET);
	if (Check(result != 0, "fseek()")) {
		return nullptr;
	}

	char* text = new char[fileSize+1]; // TODO: Fix memory leak...

	// TODO: Read count is wrong when not using UNIX line end character
	std::uint64_t readCount
		= std::fread(static_cast<char*>(text), sizeof(char), fileSize, file);
	if (Check(readCount != fileSize, "fread()")) {
		std::ferror(file);
		std::fprintf(stderr, "Read count: %lu is not file size %zu\n", readCount, fileSize);
		return nullptr;
	}

	std::printf("Last char is: %c\n", text[fileSize]);
	text[fileSize] = '\0';
	if (text[fileSize] == '\0')
		std::printf("Good!\n");
	else
		std::printf("Bad!\n");

	std::fclose(file);
	std::printf("Successfully read file of size %zi\n", fileSize);

	return text;
}
} // FG24 namespace
