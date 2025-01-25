#pragma once
#include <cstddef>
#include <cstdio>
#include "Filepath.hpp"
namespace FG24 {
	// Use std::unique_ptr? use std::string? use own wrapper class?
	// Own wrapper class could contain a dynamically allocated fixed size const char*
	// And then on destruction it could free that memory. 

namespace File {
enum class ErrorCode {
	Ok,
	NoFile, // Failed to open file stream
	LoadObjFailed
};

// RAII c FILE*. This class is local to this file
class FileStream {
public:
	FileStream(Filepath path, const char* mode);
	~FileStream();
	bool IsValid();

	FILE* ptr = nullptr;
};

// Load LF formatted ANSI text files and leaks some memory to boot!
const char* LoadTextFile(const char* path);
void FormatFilePath(char* s, std::size_t size);

} // namespace File
} // namespace FG24

