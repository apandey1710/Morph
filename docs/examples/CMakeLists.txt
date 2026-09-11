cmake_minimum_required(VERSION 3.25)
project(MorphTutorials LANGUAGES C CXX)
set(CMAKE_CXX_EXTENSIONS OFF)
include(FetchContent)
set(BUILD_EXAMPLES OFF CACHE BOOL "Build raylib examples")
FetchContent_Declare(raylib
    URL https://github.com/raysan5/raylib/archive/refs/tags/6.0.tar.gz
    URL_HASH SHA256=2b3ee1e2120c7a0796b33062c7e9a694dd8a8caa56a96319ac8c8ecf54a90d0b
)
FetchContent_MakeAvailable(raylib)
foreach(lesson IN ITEMS 01-single-ray 02-ray-fan 03-wall-strips)
    string(SUBSTRING "${lesson}" 0 2 number)
    add_executable(tutorial_${number} ${lesson}.cpp)
    target_compile_features(tutorial_${number} PRIVATE cxx_std_20)
    target_link_libraries(tutorial_${number} PRIVATE raylib)
endforeach()
