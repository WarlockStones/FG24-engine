#pragma once
#include <vector>
#include <cstddef>
#include "Entity.hpp"

class EntityManager {
public:
	void Init();
	int GetEntityCount();
	const std::string& GetEntityNameAtIndex(std::size_t index);

	std::optional<std::reference_wrapper<Entity>> selectedEntity;
	std::vector<Entity> entities;
private:
};
