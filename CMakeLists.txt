cmake_minimum_required(VERSION 3.25)

project(Morph VERSION 0.1.0 LANGUAGES C CXX)
include(CTest)

# Compile every target, including fetched dependencies, as C++20. Catch2 only
# requests C++14, and Clang targeting MSVC defaults to C++14, so without this
# Catch2 is built without the C++17 std::byte support its headers declare to
# our C++20 tests, and the tests fail to link.
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
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

# SDL 3.4.16, verified by SHA-256. Built statically so the executable needs no
# SDL shared library at runtime; the SDL3_test helper library is not needed.
# Video backends are chosen at configure time from the installed X11/Wayland
# development packages, so check the configure summary if SDL_Init fails.
set(SDL_SHARED OFF CACHE BOOL "Build a shared version of SDL3")
set(SDL_STATIC ON CACHE BOOL "Build a static version of SDL3")
set(SDL_TEST_LIBRARY OFF CACHE BOOL "Build the SDL3_test library")
FetchContent_Declare(SDL3
    URL https://github.com/libsdl-org/SDL/releases/download/release-3.4.16/SDL3-3.4.16.tar.gz
    URL_HASH SHA256=7322236cd12090c3eb40b9728be4d49c76f66ad17d04369584d4ecad5cf77c68
)
FetchContent_MakeAvailable(SDL3)

# Catch2 v3.8.1, verified by SHA-256; made available only when tests are built.
FetchContent_Declare(Catch2
        URL https://codeload.github.com/catchorg/Catch2/tar.gz/refs/tags/v3.8.1
        URL_HASH SHA256=18b3f70ac80fccc340d8c6ff0f339b2ae64944782f8d2fca2bd705cf47cadb79
)

add_subdirectory(libs/math)
add_subdirectory(libs/util)
add_subdirectory(libs/graphics)
add_subdirectory(libs/io)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()

add_executable(morph src/main.cpp)
# raylib stays linked until main.cpp is ported to SDL3.
target_link_libraries(morph PRIVATE raylib SDL3::SDL3 morph::math morph::util morph::graphics morph::io)
