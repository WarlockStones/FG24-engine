#include "files.hpp"
#include <cstdio>
#include <cstdint>
#include <utility>

namespace FG24 {
const char* LoadTextFile(const char* path) {
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

	char* text = new char[fileSize];

	std::uint64_t readCount
	  =	std::fread(static_cast<char*>(text), sizeof(char), fileSize, file);
	if (Check(readCount != fileSize, "fread()")) {
		return nullptr;
	}

	std::fclose(file);
	std::printf("Successfully read file of size %zi\n", fileSize);

	return std::move(text);
}
} // FG24 namespace
