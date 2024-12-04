#include "Entity.hpp"
#include <optional>
#include <utility>
#include <iostream>

// Try to find key in vector of EntityProperties
std::vector<EntityProperty>::iterator FindKey(std::vector<EntityProperty>& properties, const std::string& key) {
	return std::find_if(properties.begin(), properties.end(),
		[&](EntityProperty& property) {
			return property.key == key;
	});
}

std::vector<EntityProperty>::const_iterator FindKey(const std::vector<EntityProperty>& properties, const std::string& key) {
	return std::find_if(properties.begin(), properties.end(),
		[&](const EntityProperty& property) {
			return property.key == key;
	});
}

const std::string& GetValueOrDefault(const std::vector<EntityProperty>& properties, const std::string& key) {
	// Print that key has no default value and return the defaultValue ""
	auto it = FindKey(properties, key);

	if (it != properties.end()) {
		return it->value;
	}

	if (DefaultPropertyPair.contains(key.c_str())) { // TODO: Fix need to c_str();
		return DefaultPropertyPair[key.c_str()]; // Return to temporary ref! WARNING!
	} else {
		std::cout<<"Warning: DefaultPropertyPair does not include \'"<<key<<"!\n";
		return EntityPropertyValues::defaultValue;
	}
}


const std::string& Entity::GetName() const {
	if (!hasCachedName) {
	    // TODO: How to assign value from reference, aka. copy?
		cachedName = GetValueOrDefault(properties, EntityPropertyKeys::className);
		hasCachedName = true; // bool is mutable
	}


	return cachedName;
}

void Entity::AddProperty(std::string key, std::string value) {
	auto it = FindKey(properties, key);

	if (it != properties.end()) {
		// TODO: it->SetValue(std::move(value));
		it->value = std::move(value);
	} else {
		properties.emplace_back(std::move(key), std::move(value));
	}
}

std::optional<std::reference_wrapper<std::string>> Entity::GetValue(const std::string& key) const {
	// TODO make this find_if a function
	auto it = FindKey(properties, key);

	if (it != properties.end()) {
		// Just having some "fun" with modern c++ standard library features
		return std::optional<std::reference_wrapper<std::string>>(const_cast<std::string&>(it->value));
	} else {
		return std::nullopt;
	}
}
