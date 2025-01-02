#pragma once

namespace FG24 {

enum class MessageType {
	Int,
	Float
};

class Message {
public:
  Message(MessageType type) : type(type){};
  virtual ~Message() = default;
  MessageType type;
};

class IntMessage : public Message {
public:
  IntMessage(int value) : Message(MessageType::Int), value(value) {};
  ~IntMessage() = default;
  int value{};
};

class FloatMessage : public Message {
public:
  FloatMessage(float value) : Message(MessageType::Float), value(value) {};
  ~FloatMessage() = default;
  float value{};
};


} // namespace FG24
