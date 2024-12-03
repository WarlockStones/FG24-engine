#pragma once
#include <string>
#include <vector>
#include <QMetaType>
#include <algorithm>
#include <optional>

namespace EntityPropertyKeys {
	constexpr std::string classname = "classname"; // Tells engine what to spawn
	constexpr std::string origin = "origin";       // Spawn location. 3 floats xyz
}

namespace EntityPropertyValues {
	constexpr std::string defaultValue = "";
	constexpr std::string noClassname = "undefined";
	constexpr std::string spawnFlags = "spawnflags";
}

struct EntityProperty {
	// TODO introduce functions
	std::string key{};
	std::string value{};
};

class Entity {
public:
	std::vector<EntityProperty> properties{};

    void AddProperty(std::string key, std::string value);
    std::optional<std::string*> GetValue(const std::string& key) const;

	// Model for editor view
    // Transform for editor. Probably just a vec3 origin and mat4x4 rotation
  
	// Should it have a name?

};

Q_DECLARE_METATYPE(Entity) // Allows Entity to be stored in a QVariant

