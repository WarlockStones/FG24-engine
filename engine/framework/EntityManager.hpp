#pragma once
#include <cstddef>
#include <vector>
#include "framework/Entity.hpp"
namespace FG24 {
class EntityManager {
public:
	int AddEntity(const Mesh& mesh, std::uint32_t shaderID);
	void RemoveEntity(int entityID);

	std::vector<Entity>& GetEntities();
private:
	std::vector<Entity> m_entities;
};

} // namespace FG24