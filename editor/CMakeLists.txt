project(WorldEditor)

set(CMAKE_CXX_STANDARD 20)

# CMAKE_SOURCE_DIR will be top directory
include_directories(${CMAKE_SOURCE_DIR}/includes)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)


add_executable(WorldEditor 
	Main.cpp
	EntityModel.cpp
	EntityManager.cpp
	MainWindow.cpp
	EntityDelegate.cpp
	Entity.cpp
	EntityPropertyModel.cpp
	EntityProperty.cpp
)

target_link_libraries(WorldEditor PRIVATE Qt6::Core Qt6::Widgets)

if (WIN32)
# Run windeployqt on target file to make Qt find all necessary windows platform plugins and dependencies at that directory
	add_custom_command(TARGET WorldEditor POST_BUILD 
		COMMAND Qt6::windeployqt
		ARGS $<TARGET_FILE:WorldEditor>
	)
endif()

set_target_properties(WorldEditor PROPERTIES AUTOMOC TRUE)
