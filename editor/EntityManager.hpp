#pragma once
#include <vector>
#include <cstddef>
#include "Entity.hpp"

class EntityManager {
public:
	void Init();
	int GetEntityCount();
	const std::string& GetEntityNameAtIndex(std::size_t index);

	Entity& GetSelectedEntity();

	std::vector<Entity> entities;
private:
	std::optional<std::reference_wrapper<Entity>> SelectedEntity;
};
