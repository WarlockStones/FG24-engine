#pragma once
#include <queue>

namespace FG24 {
class Message;

class ExampleManager {
public:
	void QueueMessage(Message* message); // Add a new message to the queue
    void ProcessMessages(); // Process all messages
private:
    void ProcessMessage(Message* message); // Process a single message
	std::queue<Message*> messages; 
};
} // namespace FG24
