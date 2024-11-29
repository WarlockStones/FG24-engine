#pragma once
#include <vector>
#include <cstddef>
#include "Entity.hpp"

class EntityManager {
public:
	void Init();
	int GetEntityCount();
    const char* GetEntityNameAtIndex(std::size_t index);

	// TODO: Events so that the entityModel can listen in for any change

private:
	std::vector<Entity> entities;
};
