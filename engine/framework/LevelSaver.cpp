#include "LevelSaver.hpp"
#include "utils/Filepath.hpp"
#include "utils/File.hpp"
#include "framework/EntityManager.hpp"
#include "framework/Entity.hpp"

namespace FG24 {
namespace LevelSaver {

// TODO: Do error checking
static constexpr std::uint32_t version = 1;
static constexpr std::uint32_t magic = 0x6C76656C; // "levl" ASCII to hex
ErrorCode SaveEntities(EntityManager& entityManager) {
	// magic word: levl
	// version
    // Data begin:
    // num entities EntityManager::GetEntities().size();
    // For each entity save its configuration
    //   meshId, shaderId, textureId, name, Transform 
    //   This is done by calling the WriteTo of the entity->WriteTo();

    File::FileStream fs("../../assets/default.lvl", "wb");
	if (!fs.IsValid()) {
		return ErrorCode::NoFile;
	}
	std::size_t n = 0;
	auto& entities = entityManager.GetEntities(); // Annoying objects
	std::uint32_t numEntities = static_cast<std::uint32_t>(entities.size());
	n += std::fwrite(&magic, sizeof(std::uint32_t), 1, fs.ptr);
	n += std::fwrite(&version, sizeof(std::uint32_t), 1, fs.ptr);
	n += std::fwrite(&numEntities, sizeof(std::uint32_t), 1, fs.ptr);
	static constexpr std::size_t expected = 3;
	if (n != expected) {
	  std::fprintf(stderr, "Error: Failed to save header for saved entities file\n");
	  return ErrorCode::Fail;
	}
	for (auto* e : entities) {
		if (e->WriteTo(fs.ptr) == false) {
			return ErrorCode::Fail;
		}
	}

	return ErrorCode::Ok;
}

ErrorCode LoadEntities(EntityManager& entityManager) {
    File::FileStream fs("../../assets/default.lvl", "rb");
	if (!fs.IsValid()) {
		return ErrorCode::NoFile;
	}
	std::size_t n = 0;
	std::uint32_t magicBuf = 0;
	std::uint32_t versionBuf = 0;
	std::uint32_t numEntities = 0;
	n += std::fread(&magicBuf, sizeof(std::uint32_t), 1, fs.ptr);
	n += std::fread(&versionBuf, sizeof(std::uint32_t), 1, fs.ptr);
	n += std::fread(&numEntities, sizeof(std::uint32_t), 1, fs.ptr);
	static constexpr std::size_t expected = 3;
	if (n != expected) {
		std::fprintf(stderr, "Error: Failed to load header for saved entities file\n");
		return ErrorCode::Fail;
	}
	if ((magicBuf != magic) || (versionBuf != version)) {
	    std::fprintf(stderr, "Error: save entities file has invalid header");
		return ErrorCode::Fail;
	}
	
	std::vector<Entity*> entities;
	entities.reserve(numEntities);
	for (std::uint32_t i = 0; i < numEntities; ++i) {
		Entity* e = new Entity;
		entities.push_back(e);
		bool ok = e->ReadFrom(fs.ptr);
		if (!ok) {
		    std::fprintf(stderr, "LoadEntities failed to load an entity!\n");
			for (Entity* entity : entities) {
				delete entity;
			}
			return ErrorCode::Fail;
		}
	}

	// Load entities into EntityManager 
	entityManager.ReplaceEntities(entities);
	
	return ErrorCode::Ok;
}


} // namespace LevelSaver
} // namespace FG24
