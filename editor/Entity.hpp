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

    void AddProperty(std::string_view key, std::string_view value);
    std::optional<std::string_view> GetValue(std::string_view) const;

	const std::string& GetName() const;

	// Model for editor view
    // Transform for editor. Probably just a vec3 origin and mat4x4 rotation
  
	// Should it have a name?
private:
	mutable std::optional<std::string> cachedName;

};
Q_DECLARE_METATYPE(Entity) // Allows Entity to be stored in a QVariant

