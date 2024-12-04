#include "EntityProperty.hpp"

std::unordered_map<std::string_view, std::string_view> DefaultPropertyPair = {
	{EntityPropertyKeys::className, EntityPropertyValues::noClassName},
	{EntityPropertyKeys::origin, EntityPropertyValues::noOrigin}
	// {EntityPropertyKeys::, EntityPropertyValues::},
};

EntityProperty::EntityProperty(std::string key, std::string value) 
: key(key), value(value) {
}
