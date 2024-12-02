#pragma once
#include <cstdint>

namespace FG24 {
namespace Memory {

// Change to "GetAvailableKilobytes"
bool HasMemoryAvailable(std::uint64_t mebibytes);
}

}