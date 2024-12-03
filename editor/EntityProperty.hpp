namespace EntityPropertyKeys {
	// constexpr std::string is bugged on msvc 17.20
	// Using std::string_view would require explicit string construct i.e. e.AddProperty(std::string(EntityProperty::classname))
	// But this is not "extensive use of c++ std library"!!
	constexpr char className[] = "classname";
	constexpr char origin[] = "origin";
};

// Map?? Keys to default values

namespace EntityPropertyValues {
	constexpr char defaultValue[] = "";
	constexpr char noClassname[] = "undefined";
	constexpr char noOrigin[] = "0 0 0";
	constexpr char noRotation[] = "0 0 0";
	constexpr char spawnFlags[] = "spawnflags";
}

struct EntityProperty {
	std::string key{};
	std::string value{};

	// TODO introduce functions
	// Make key and value private and use public functions instead?

	// As static member or function?
	/*
	* FindEntityProperty(const std::vector<EntityProperty>& properties, const std::string& key) {
	auto it = std::find_if(properties.begin(), properties.end(),
						[&](EntityProperty& property){
			return property.key == key;
	});
	*/
}
