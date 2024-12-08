#pragma once
#include <cstddef>
#include "Filepath.hpp"
namespace FG24 {
	// Use std::unique_ptr? use std::string? use own wrapper class?
	// Own wrapper class could contain a dynamically allocated fixed size const char*
	// And then on destruction it could free that memory. 

// LoadOBJtoMeshData
struct MeshData;
namespace File {
	// Load LF formatted ANSI text files and leaks some memory to boot!
	const char* LoadTextFile(const char* path);
	void FormatFilePath(char* s, std::size_t size);

	// An awfully specific function
	MeshData LoadObjToMeshData(Filepath filepath);
	MeshData OldParseObj(const char* path); // Just for archiving sake
};

} // namespace FG24
