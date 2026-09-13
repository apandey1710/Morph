add_library(morph_graphics STATIC src/graphics.cpp)
add_library(morph::graphics ALIAS morph_graphics)

target_include_directories(morph_graphics PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_compile_features(morph_graphics PUBLIC cxx_std_20)
target_link_libraries(morph_graphics PUBLIC raylib)
