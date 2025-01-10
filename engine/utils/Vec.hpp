#pragma once

namespace FG24 {
// Keep vectors simple because I want to store them to disk with minimal space
struct Vec3 {
public:
  Vec3() = default;
  Vec3(float x, float y, float z);
  ~Vec3() = default;
  float x{};
  float y{};
  float z{};
};

struct Vec2 {
public:
  Vec2() = default;
  Vec2(float x, float y);
  ~Vec2() = default;
  float x{};
  float y{};
};
} // namespace FG24
