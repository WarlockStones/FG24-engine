#pragma once
#include <unordered_map>
#include <string>

namespace EntityPropertyKeys {
	// constexpr std::string is bugged on msvc 17.20
	// std::string_view is 
	constexpr std::string_view className = "classname";
	constexpr std::string_view origin = "origin";
	constexpr std::string_view test = "classname";
};

// Map?? Keys to default values

namespace EntityPropertyValues {
	constexpr std::string_view defaultValue = "";
	constexpr std::string_view noClassName = "undefined";
	constexpr std::string_view noOrigin = "0 0 0";
	constexpr std::string_view noRotation = "0 0 0";
	constexpr std::string_view spawnFlags = "spawnflags";
}

extern std::unordered_map<std::string_view, std::string_view> DefaultPropertyPair;

struct EntityProperty {
	std::string key{};
	std::string value{};

	EntityProperty(std::string key, std::string value);
	// TODO introduce functions
	// Make key and value private and use public functions instead?

	/* 
	// As static member or function?
	FindEntityProperty(const std::vector<EntityProperty>& properties, const std::string& key) {
	auto it = std::find_if(properties.begin(), properties.end(),
						[&](EntityProperty& property){
			return property.key == key;
	});
	*/
};
