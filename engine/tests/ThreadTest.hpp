#pragma once
// Last changed 2025-01-04
#include <cstdio>
#include "Globals.hpp"
#include "utils/ThreadPool.hpp"

namespace ThreadTest {
using namespace FG24;

constexpr std::size_t arrSize = 1024;
constexpr std::size_t chunkSize = arrSize/8;
int testArr[arrSize]{};

struct IncrementTaskData {
    std::uint32_t start;
};


void IncrementTask(std::any taskData) {
	// Unpack the data
    std::printf("Running increment task\n");
	IncrementTaskData& data = std::any_cast<IncrementTaskData&>(taskData);
	std::uint32_t start = data.start;
	std::uint32_t end = start + chunkSize;
	
	// Cap end so we don't run over array
	end = (end > arrSize) ? arrSize : end;
	
	for (std::size_t i = start; i < end; ++i) {
		testArr[i] += 1;
		std::printf("%d ", testArr[i]);
	}
	std::printf("\n");
}

void RunIncrementTest() {
    for (std::size_t i = 0; i < arrSize; ++i) {
		testArr[i] = i;
	}
  
	std::printf("testArr 0: %d. TestArr 1023: %d\n", testArr[0], testArr[1023]);

	FG24::g_runGameLoop = true;
	FG24::ThreadPool::Init();

	for (std::size_t i = 0; i < 8; i++) {
		IncrementTaskData data;
		data.start = i * chunkSize;
		FG24::ThreadPool::Submit(FG24::Task(&IncrementTask, data));
	}
}


} // namespace ThreadTest
