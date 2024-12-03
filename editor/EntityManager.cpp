#include "EntityManager.hpp"
#include <iostream>
#include "Entity.hpp"

void EntityManager::Init() {
	for (int i = 0; i < 10; i++) {
		entities.push_back(Entity());
		entities.back().AddProperty(EntityPropertyKeys::classname, "teacup");

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

const char* EntityManager::GetEntityNameAtIndex(std::size_t index) {
  /* DEPRECATED
	 // TODO FIX THIS NOW THAT YOU HAVE INTRODUCED PROPERTIES
	if (index < entities.size()) {
		return entities[index].name.c_str();
	}
  */
	
	return "e";
}
