#pragma once
#include <cstddef>
#include <vector>
#include "renderer/Mesh.hpp"
namespace FG24 {
class Entity;
class EntityManager {
public:
	~EntityManager();
	Entity* CreateEntity(const Mesh* mesh, std::uint32_t shaderId, const char* name);
	void DeleteEntity(Entity* toDelete);
	std::vector<Entity*>& GetEntities();

	// Used when loading new entities from a save
    void ReplaceEntities(const std::vector<Entity*>& newEntities);
private:
	std::vector<Entity*> m_entities;
};

} // namespace FG24
