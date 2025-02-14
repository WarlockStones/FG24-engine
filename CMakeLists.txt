cmake_minimum_required(VERSION 3.21)
project(FG24)

enable_testing()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")

### End of preamble ###
add_subdirectory(${CMAKE_SOURCE_DIR}/engine)
# add_subdirectory(${CMAKE_SOURCE_DIR}/editor)

# TODO: Test on windows
add_custom_target(run
  ./engine/engine
)

add_custom_target(runed # "run editor"
  ./editor/WorldEditor
)
