#include <vector>
#include <algorithm>
#include "EntityManager.hpp"
#include "renderer/Shader.hpp"
#include "framework/Entity.hpp"

namespace FG24 {
static int availableID = 0;
int EntityManager::AddEntity(const Mesh& mesh, std::uint32_t shaderID) {
	m_entities.emplace_back(Entity(mesh, shaderID, 0, availableID)); // Texture is just 0 for now
	return availableID++;
}
void EntityManager::RemoveEntity(int entityID) {
	m_entities.erase(std::remove_if(m_entities.begin(), m_entities.end(),
		[entityID](const Entity& e) {
			return e.GetID() == entityID;

	}), m_entities.end());
}
std::vector<Entity>& EntityManager::GetEntities() {
	return m_entities;
}
} // namespace FG24