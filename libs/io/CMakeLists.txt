add_library(morph_io STATIC src/io.cpp)
add_library(morph::io ALIAS morph_io)

target_include_directories(morph_io PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_compile_features(morph_io PUBLIC cxx_std_20)
