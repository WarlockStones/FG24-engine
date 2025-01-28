#include <vector>
#include <algorithm>
#include <cassert>
#include <cstddef>
#include "EntityManager.hpp"
#include "renderer/Shader.hpp"
#include "framework/Entity.hpp"
#include "renderer/Mesh.hpp"

namespace FG24 {
EntityManager::~EntityManager() {
	for (Entity* e : m_entities) {
		delete e;
	}
}

Entity* EntityManager::CreateEntity(const Mesh* mesh, std::uint32_t shaderId, const char* name = "Entity") {
	Entity* e = new Entity(mesh, shaderId, 0, name); // Texture just 0 for now
	m_entities.push_back(e);
	return e;
}

void EntityManager::DeleteEntity(Entity* toRemove) {
	assert(toRemove != nullptr);
	for (std::size_t i = 0; i < m_entities.size(); ++i) {
		if (m_entities[i] == toRemove) {
			delete m_entities[i];
			m_entities.erase(m_entities.begin() + i);
		}
	}
}

std::vector<Entity*>& EntityManager::GetEntities() {
	return m_entities;
}
} // namespace FG24
