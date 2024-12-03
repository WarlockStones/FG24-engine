#include "Entity.hpp"
#include <optional>
#include <utility>

#include <iostream>

void Entity::AddProperty(std::string key, std::string value) {
    // TODO: Add find function.
    // Error: No matching for 'operator==' operand types are 'EntityProperty'
    //        and 'const std::__cxx11::basic_string<char>'
	auto it = std::find(properties.begin(), properties.end(), key);

	if (it != properties.end()) {
		// TODO: it->SetValue(std::move(value));
		it->value = std::move(value);
	} else {
		properties.emplace_back(std::move(key), std::move(value));
	}
}

std::optional<std::string*> Entity::GetValue(const std::string& key) const {
	const auto it = std::find(properties.begin(), properties.end(), key);
	if (it != properties.end()) {
	  std::cout <<"addr: "<<&it->value<<'\n';
	  // return std::optional<std::reference_wrapper<std::string*>> {&it->value};
	}

    return std::nullopt;
}
