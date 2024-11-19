#pragma once
namespace FG24 {
	// Use std::unique_ptr? use std::string? use own wrapper class?
	// Own wrapper class could contain a dynamically allocated fixed size const char*
	// And then on destruction it could free that memory. 

	using TextFileContent = const char*; // A dynamically allocated c-string

	TextFileContent LoadTextFile(const char* path);
}
