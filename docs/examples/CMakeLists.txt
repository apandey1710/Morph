cmake_minimum_required(VERSION 3.25)
project(MorphTestingTutorial LANGUAGES CXX)
set(CMAKE_CXX_EXTENSIONS OFF)
include(CTest)

add_library(morph_io STATIC io.cpp)
add_library(morph::io ALIAS morph_io)
target_include_directories(morph_io PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_compile_features(morph_io PUBLIC cxx_std_20)

if(BUILD_TESTING)
    include(FetchContent)
    FetchContent_Declare(Catch2
        URL https://codeload.github.com/catchorg/Catch2/tar.gz/refs/tags/v3.8.1
        URL_HASH SHA256=18b3f70ac80fccc340d8c6ff0f339b2ae64944782f8d2fca2bd705cf47cadb79
    )
    FetchContent_MakeAvailable(Catch2)
    add_executable(morph_io_tests io_tests.cpp)
    target_link_libraries(morph_io_tests PRIVATE morph::io Catch2::Catch2WithMain)
    list(APPEND CMAKE_MODULE_PATH "${catch2_SOURCE_DIR}/extras")
    include(Catch)
    catch_discover_tests(morph_io_tests)
endif()
