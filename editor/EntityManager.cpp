#include "EntityManager.hpp"
#include <iostream>
#include "Entity.hpp"
#include <cassert>
#include <string>
#include <utility>

void EntityManager::Init() {
	for (int i = 0; i < 10; i++) {
		entities.push_back(Entity());
		entities.back().AddProperty(EntityPropertyKeys::className, "teacup");

		std::string positionOffset = std::to_string(i * 10);
		std::string origin = positionOffset;
		origin += " 0 ";
		origin += positionOffset;
		
		entities.back().AddProperty(EntityPropertyKeys::origin, origin);
	}
}

int EntityManager::GetEntityCount() {
	return entities.size();
}

const std::string& EntityManager::GetEntityNameAtIndex(std::size_t index) {
	assert(index < entities.size());
	return entities[index].GetName();
}
