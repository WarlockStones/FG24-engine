#pragma once
#include <cstddef>
#include <vector>
#include "renderer/Mesh.hpp"
namespace FG24 {
class Entity;
class EntityManager {
public:
	~EntityManager();
	Entity* CreateEntity(const Mesh& mesh, std::uint32_t shaderId);
	void DeleteEntity(Entity* toDelete);
	std::vector<Entity*>& GetEntities();
private:
	std::vector<Entity*> m_entities;
};

} // namespace FG24