#include "EntityManager.hpp"
#include <iostream>

void EntityManager::Init() {
	for (int i = 0; i < 10; i++) {
		entities.push_back(Entity());
		std::string name = "entity_";
		name += std::to_string(i);
		entities[i].name = name;
	}
	for (const auto& e : entities) {
		std::cout<<"entity name: "<<e.name<<'\n';
	}
}

int EntityManager::GetEntityCount() {
	return entities.size();
}

const char* EntityManager::GetEntityNameAtIndex(std::size_t index) {
	if (index < entities.size()) {
		return entities[index].name.c_str();
	}
	
	return "";
}
