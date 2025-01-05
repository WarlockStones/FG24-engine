#include "ThreadPool.hpp"
#include <cstddef>
#include <thread>
#include <mutex>
#include <queue>
#include <cstdio>
#include <optional>

#include "Globals.hpp"

namespace FG24 {
namespace ThreadPool {
static std::vector<std::thread> threads{};
static std::queue<Task> tasks{}; // Can be optimised using a tree
static std::mutex taskMutex;

void Init() {
	// Reserving one thread for main to avoid oversubscription. (Is this correct?)
	auto numThreads = std::thread::hardware_concurrency() -1;
	for (std::size_t i = 0; i < numThreads; ++i) {
		std::printf("Created thread %d\n", i);
		threads.push_back(std::thread([](){
			std::optional<Task> t = std::nullopt;
			// TODO: Using this global means I have to set it to true before 
			// calling this Init function. Look for alternative to closing this
			while (g_runGameLoop) { 
				// Get a new task from the queue in a thread safe manner
				taskMutex.lock();
				if (!tasks.empty()) {
				    std::printf("Grabbed a work!\n");
					t = std::make_optional<Task>(tasks.front()); // Copy to local space
					tasks.pop();
				}
				taskMutex.unlock();

				// Do work of task
				if (t.has_value()) {
					t.value().Invoke(); // Run the work function
					t.reset(); // Clear contained value
					t = std::nullopt;
				}
			}
		}));
	}
}

void Submit(Task work) {
	std::lock_guard<std::mutex> guard(taskMutex);
	tasks.push(work);
}

void Close() {
    for (auto& t : threads) {
		t.join();
		std::printf("Joined a thread.\n");
	}
}

} // namespace ThreadPool
} // namespace FG24
