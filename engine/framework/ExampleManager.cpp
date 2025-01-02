#include "ExampleManager.hpp"
#include "Message.hpp"
#include <cstdio>

namespace FG24 {
 
void ExampleManager::QueueMessage(Message* message) {
	messages.push(message);
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
} // namespace FG24
