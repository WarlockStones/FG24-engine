#pragma once
#include <queue>
#include <mutex>
#include <thread>
namespace FG24 {
class Message;

class ExampleManager {
public:
    ExampleManager() = default;
	void QueueMessage(Message* message); // Add a new message to the queue
    void StartThread();
    ~ExampleManager();
private:
    void Update();
    void ProcessMessages(); // Process all messages
    void ProcessMessage(Message* message); // Process a single message
	std::queue<Message*> messages; 
    std::mutex messageQueueMutex;
    std::thread thread;
};
} // namespace FG24
