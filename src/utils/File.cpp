#include "File.hpp"
#include <cstdio>
#include <cstdint>
#include <utility>

namespace FG24 {
TextFileContent LoadTextFile(const char* path) {
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

	char* text = new char[fileSize]; // TODO: Fix memory leak...

	// TODO: Read count is wrong when not using UNIX line end character
	// But changing it to unix causes the OpenGL shader compilation to fail because of unexpected token $
	std::uint64_t readCount
		= std::fread(static_cast<char*>(text), sizeof(char), fileSize, file);
	if (Check(readCount != fileSize, "fread()")) {
		std::ferror(file);
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
