#include "ExampleManager.hpp"
#include "Message.hpp"
#include <cstdio>
#include <mutex>
#include <thread>

#include "Globals.hpp" // For g_runGameLoop

namespace FG24 {
 
void ExampleManager::QueueMessage(Message* message) {
    // std::this_thread::sleep_for(std::chrono::seconds(2)); // Simulate delay for testing
	std::lock_guard<std::mutex> guard(messageQueueMutex); // RAII mutex lock/unlock
	messages.push(message);
}

void ExampleManager::StartThread() {
	thread = std::thread(&ExampleManager::Update, this);
}

void ExampleManager::Update() {
	// TODO: Add some sort of tick-rate
    while (g_runGameLoop) {
		std::this_thread::sleep_for(std::chrono::seconds(1)); // To not blow CPU usage to 100%
		std::printf("ExampleManager tick...\n");
		ProcessMessages();
	}
}

void ExampleManager::ProcessMessages() {
	while (messages.size()) {
		Message* m = messages.front();
		ProcessMessage(m);
		messages.pop();
		delete m;
	}
}

void ExampleManager::ProcessMessage(Message* message) {
	switch(message->type) {
		case MessageType::Int: {
		auto* m = dynamic_cast<IntMessage*>(message);
		std::printf("ExampleManager recieved an IntMessage: %d\n", m->value);
		break;
		}

		case MessageType::Float: {
		auto* m = dynamic_cast<FloatMessage*>(message);
		std::printf("ExampleManager recieved a FloatMessage: %f\n", m->value);
		break;
		}
	}
}

ExampleManager::~ExampleManager() {
	if (thread.joinable()) {
		thread.join();
	}
}

} // namespace FG24
