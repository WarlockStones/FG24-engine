#pragma once
#include <cstdint>
#include <functional>
#include <any>

namespace FG24 {
using TaskFunc = std::function<void(std::any)>; 

class Task {
public:
	Task(TaskFunc callback, std::any data)
		: callback(callback), data(data) {}

	// Run the callback with the data provided
	void Invoke() {
		callback(data);
	}

	~Task() = default;
private:
	TaskFunc callback{}; // Function with TaskData parameter
	std::any data{};
};

namespace ThreadPool {
	void Init();
	void Submit(Task work);
    void Close();
};
} // namespace FG24
