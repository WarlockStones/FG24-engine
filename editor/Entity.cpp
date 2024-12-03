#include "Entity.hpp"
#include <optional>
#include <utility>

#include <iostream>

void Entity::AddProperty(std::string key, std::string value) {
	// TODO make this find_if a function
	auto it = std::find_if(properties.begin(), properties.end(),
						[&](EntityProperty& property){
			return property.key == key;
	});

	if (it != properties.end()) {
		// TODO: it->SetValue(std::move(value));
		it->value = std::move(value);
	} else {
		properties.emplace_back(std::move(key), std::move(value));
	}
}

std::optional<std::reference_wrapper<std::string>> Entity::GetValue(const std::string& key) const {
	// TODO make this find_if a function
	const auto it = std::find_if(properties.begin(), properties.end(),
						[&](const EntityProperty& property){
			return property.key == key;
	});

	if (it != properties.end()) {
		return std::optional<std::reference_wrapper<std::string>>(const_cast<std::string&>(it->value));
	} else {
		return std::nullopt;
	}
}
