#include "Entity.hpp"
#include <optional>
#include <utility>
#include <iostream>

// Try to find key in vector of EntityProperties
std::vector<EntityProperty>::iterator FindKey(std::vector<EntityProperty>& properties, std::string_view key) {
	return std::find_if(properties.begin(), properties.end(),
		[&](EntityProperty& property) {
			return property.key == key;
	});
}

std::vector<EntityProperty>::const_iterator FindKey(const std::vector<EntityProperty>& properties, std::string_view key) {
	return std::find_if(properties.begin(), properties.end(),
		[&](const EntityProperty& property) {
			return property.key == key;
	});
}

std::string_view GetValueOrDefault(const std::vector<EntityProperty>& properties, std::string_view key) {
	auto it = FindKey(properties, key);

	if (it != properties.end()) {
		return it->value;
	}

	if (DefaultPropertyPair.contains(key)) {
		return DefaultPropertyPair[key]; 
	} else {
		std::cout<<"Warning: '"<<key<<"' has no default property!\n";
		return EntityPropertyValues::defaultValue;
	}
}


// Returning string_view in getter is bad. String_view becomes invalid as soon as underlying data is changed
// Getter functions should return const T&
const std::string& Entity::GetName() const {
	if (!cachedName.has_value()) {
		cachedName = GetValueOrDefault(properties, EntityPropertyKeys::className);
	}

	return cachedName.value();
}

void Entity::AddProperty(std::string_view key, std::string_view value) {

	auto it = FindKey(properties, key);

	if (it != properties.end()) {
		it->value = std::string(value);
	} else {
		// Construct property
		properties.emplace_back(EntityProperty(std::string{ key }, std::string{ value }));
	}
}

std::optional<std::string_view> Entity::GetValue(std::string_view key) const {
	auto it = FindKey(properties, key);

	if (it != properties.end()) {
		return std::optional<std::string_view>(it->value);
	} else {
		return std::nullopt;
	}
}
