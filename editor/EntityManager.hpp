#pragma once
#include <vector>
#include <cstddef>
#include "Entity.hpp"

class EntityManager {
public:
	void Init();
	int GetEntityCount();
    const char* GetEntityNameAtIndex(std::size_t index);

	std::vector<Entity> entities;
private:
};
