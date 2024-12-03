#pragma once
#include <string>
#include <vector>
#include <QMetaType>
#include <algorithm>
#include <optional>
#include "EntityProperty.hpp"

class Entity {
public:
	std::vector<EntityProperty> properties{};

    void AddProperty(std::string key, std::string value);
    std::optional<std::reference_wrapper<std::string>> GetValue(const std::string& key) const;

	// Model for editor view
    // Transform for editor. Probably just a vec3 origin and mat4x4 rotation
  
	// Should it have a name?

};

Q_DECLARE_METATYPE(Entity) // Allows Entity to be stored in a QVariant

