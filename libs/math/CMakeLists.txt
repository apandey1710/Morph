add_library(morph_math STATIC src/math.cpp)
add_library(morph::math ALIAS morph_math)

target_include_directories(morph_math PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_compile_features(morph_math PUBLIC cxx_std_20)

