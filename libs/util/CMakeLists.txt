add_library(morph_util STATIC src/util.cpp)
add_library(morph::util ALIAS morph_util)

target_include_directories(morph_util PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_compile_features(morph_util PUBLIC cxx_std_20)

