#pragma once
#include "framework/EntityManager.hpp"

namespace FG24 {
namespace LevelSaver {
enum ErrorCode { Ok, NoFile, Fail };
	ErrorCode SaveEntities(EntityManager& entityManager);
	ErrorCode LoadEntities(EntityManager& entityManager);
} // namespace LevelSaver
} // namespace FG24
