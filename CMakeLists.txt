cmake_minimum_required(VERSION 3.25)

project(Morph VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

# Keep the dependency reproducible and skip raylib's example applications.
set(BUILD_EXAMPLES OFF CACHE BOOL "Build raylib examples")
FetchContent_Declare(raylib
    URL https://github.com/raysan5/raylib/archive/refs/tags/6.0.tar.gz
    URL_HASH SHA256=2b3ee1e2120c7a0796b33062c7e9a694dd8a8caa56a96319ac8c8ecf54a90d0b
)
FetchContent_MakeAvailable(raylib)

add_subdirectory(libs/math)
add_subdirectory(libs/util)

add_executable(morph src/main.cpp)
target_compile_features(morph PRIVATE cxx_std_20)
target_link_libraries(morph PRIVATE raylib morph::math morph::util)

