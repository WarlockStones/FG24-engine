#pragma once
#include <cstdint>

namespace FG24 {
namespace Memory {
	// Returns available RAM in metric kilobytes (available RAM / 1000)
	std::uint64_t GetAvailableKilobytes();
} // Memory
} // FG24
