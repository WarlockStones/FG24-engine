#include "Memory.hpp"
#include <cassert>
#include <cstdint>
#include <cstdlib> // For std::abort
#include <cstdio>
#ifdef _WIN32
#include <windows.h>
#endif

namespace FG24 {
namespace Memory {
#ifdef _WIN32
bool HasMemoryAvailable(std::uint64_t mebibytes) {
	MEMORYSTATUSEX statex; 
	statex.dwLength = sizeof(statex);
	// TODO: Do proper error handling
	// GlobalMemoryStatusEx returns false if failed.
	assert(GlobalMemoryStatusEx(&statex)); 

	// Return an error and let the caller handle it. Maybe just don't load the mesh or something
	
	auto remainingMebiBytes = statex.ullAvailPhys / (1024 * 1024);
	return mebibytes < remainingMebiBytes;
}

#else 
bool HasMemoryAvailable(std::uint64_t mebibytes) {
	std::fprintf(stderr, "Has memeory available is not yet implemented on non-windows machines!\n");
	std::abort();
}
#endif
}
}