#pragma once
#include <string>
#include <QMetaType>

struct Vec3 {
	float x = 0;
	float y = 0;
	float z = 0;
};

struct Entity {
	std::string name{};
	Vec3 position{};
	Vec3 rotation{};
	Vec3 scale{};
};

// Lets do it like Quake, an entity is just a collection of key-value pairs string-to-string
// You can just use std::string and std::move std::string whenever possible.
struct EntityProperty {

};

Q_DECLARE_METATYPE(Entity) // Allows Entity to be stored in a QVariant

