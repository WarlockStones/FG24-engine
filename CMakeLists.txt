cmake_minimum_required(VERSION 3.21)
project(FG24)

enable_testing()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")

include_directories(${CMAKE_SOURCE_DIR}/includes)

# 1. Look for a SDL2 package, 2. look for the SDL2 component and 3. fail if none can be found
find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)

# 1. Look for a SDL2 package, 2. Look for the SDL2maincomponent and 3. DO NOT fail when SDL2main is not available
find_package(SDL2 REQUIRED CONFIG COMPONENTS SDL2main)

find_package(SDL2_image CONFIG REQUIRED)

find_package(OpenGL REQUIRED)
add_definitions(${OPENGL_DEFINITIONS})

find_package(glm CONFIG REQUIRED)

set(IMGUI_SRC
	src/imgui/imgui.cpp
	src/imgui/imgui_demo.cpp
	src/imgui/imgui_draw.cpp
	src/imgui/imgui_impl_opengl3.cpp
	src/imgui/imgui_impl_sdl2.cpp
	src/imgui/imgui_impl_win32.cpp
	src/imgui/imgui_tables.cpp
	src/imgui/imgui_widgets.cpp
)

add_executable(FG24
	"src/Main.cpp"
	"src/Globals.cpp"
	"src/glad/gl.c"
	"src/utils/File.cpp"
	"src/framework/Session.cpp"
	"src/framework/KeyInput.cpp"
	"src/framework/Actor.cpp"
	"src/renderer/Renderer.cpp"
	"src/renderer/Triangle.cpp"
	"src/renderer/Mesh.cpp"
	"src/renderer/Material.cpp"
	"src/renderer/Texture.cpp"
	"src/renderer/Shader.cpp"
	${IMGUI_SRC}
)



target_include_directories(FG24 PRIVATE
	"src/"
	"src/imgui/"
)

# SDL2::SDL2main may or may not be available. It is e.g. required by Windows GUI applications
if(TARGET SDL2::SDL2main)
    # It has an implicit dependency on SDL2 functions, so it MUST be added before SDL2::SDL2 (or SDL2::SDL2-static)
    target_link_libraries(FG24 PRIVATE SDL2::SDL2main)
endif()

# Link to the actual SDL2 library. SDL2::SDL2 is the shared SDL library, SDL2::SDL2-static is the static SDL libarary.
target_link_libraries(FG24 PRIVATE SDL2::SDL2)
target_link_libraries(FG24 PRIVATE $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>)
target_link_libraries(FG24 PRIVATE glm::glm)
