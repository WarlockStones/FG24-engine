#include "Memory.hpp"
#include <cassert>
#include <cstdint>
#include <cstdlib> // For std::abort
#include <cstdio>

#ifdef _WIN32
#include <windows.h>
#elif __linux__
#include <sys/sysinfo.h>
#endif

namespace FG24 {
namespace Memory {
#ifdef _WIN32
std::uint64_t GetAvailableKilobytes() {
	MEMORYSTATUSEX statex; 
	statex.dwLength = sizeof(statex);

	// TODO: Do proper error handling
	// GlobalMemoryStatusEx returns false if failed.
	assert(GlobalMemoryStatusEx(&statex)); 

	// Return an error and let the caller handle it. 
	// Maybe just don't load the mesh or something
	
	return statex.ullAvailPhys / 1000;
}
#elif __linux__
std::uint64_t GetAvailableKilobytes() {
	struct sysinfo info;

	if (sysinfo(&info) == -1) {
		// TODO: Handle error
		return 0;
	} else {
		return info.freeram / 1000;
	}
}
#endif

} // Memory
} // FG24
