project(engine)

include_directories(${CMAKE_SOURCE_DIR}/includes)

# 1. Look for a SDL2 package, 2. look for the SDL2 component and 3. fail if none can be found
find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)

# 1. Look for a SDL2 package, 2. Look for the SDL2maincomponent and 3. DO NOT fail when SDL2main is not available
find_package(SDL2 REQUIRED CONFIG COMPONENTS SDL2main)

find_package(SDL2_image CONFIG REQUIRED)

find_package(OpenGL REQUIRED)
add_definitions(${OPENGL_DEFINITIONS})

find_package(glm CONFIG REQUIRED)

set(IMGUI_SOURCES
	"imgui/imgui.cpp"
	"imgui/imgui_draw.cpp"
	"imgui/imgui_impl_opengl3.cpp"
	"imgui/imgui_impl_sdl2.cpp"
	"imgui/imgui_tables.cpp"
	"imgui/imgui_widgets.cpp"
	"imgui/imgui_demo.cpp"
)

add_executable(engine
	"Main.cpp"
	"Globals.cpp"
	"glad/gl.c"
	"utils/File.cpp"
	"utils/Filepath.cpp"
	"utils/Memory.cpp"
	"utils/ThreadPool.cpp"
	"utils/Vec.cpp"
	"framework/Camera.cpp"
	"framework/CameraManager.cpp"
	"framework/Session.cpp"
	"framework/KeyInput.cpp"
	"framework/Editor.cpp"
	"framework/Entity.cpp"
	"framework/EntityManager.cpp"
	"framework/ExampleManager.cpp"
	"framework/Transform.cpp"
	"framework/Lighting.cpp"
	"framework/Light.cpp"
	"framework/LevelSaver.cpp"
	"framework/MeshManager.cpp"
	"renderer/Renderer.cpp"
	"renderer/Mesh.cpp"
	"renderer/Texture.cpp"
	"renderer/Shader.cpp"
	"utils/ObjToMdl.cpp"
	${IMGUI_SOURCES}
)

target_include_directories(engine PRIVATE
	${CMAKE_SOURCE_DIR}/engine
	${CMAKE_SOURCE_DIR}/engine/imgui
)

# Link imgui headers
# target_include_directories(engine PRIVATE /imgui)

# SDL2::SDL2main may or may not be available. It is e.g. required by Windows GUI applications
if(TARGET SDL2::SDL2main)
    # It has an implicit dependency on SDL2 functions, so it MUST be added before SDL2::SDL2 (or SDL2::SDL2-static)
    target_link_libraries(engine PRIVATE SDL2::SDL2main)
endif()

# Link to the actual SDL2 library. SDL2::SDL2 is the shared SDL library, SDL2::SDL2-static is the static SDL libarary
target_link_libraries(engine PRIVATE SDL2::SDL2)
target_link_libraries(engine PRIVATE $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>)
target_link_libraries(engine PRIVATE glm::glm)
