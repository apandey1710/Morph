FetchContent_MakeAvailable(Catch2)
list(APPEND CMAKE_MODULE_PATH "${catch2_SOURCE_DIR}/extras")
include(Catch)

add_executable(morph_io_tests io_tests.cpp)
target_link_libraries(morph_io_tests PRIVATE morph::io Catch2::Catch2WithMain)
catch_discover_tests(morph_io_tests ADD_TAGS_AS_LABELS)

add_executable(morph_util_tests util_tests.cpp)
target_link_libraries(morph_util_tests PRIVATE morph::util Catch2::Catch2WithMain)
catch_discover_tests(morph_util_tests ADD_TAGS_AS_LABELS)

add_executable(morph_raycast_tests raycast_tests.cpp)
target_link_libraries(morph_raycast_tests PRIVATE morph::graphics Catch2::Catch2WithMain)
catch_discover_tests(morph_raycast_tests ADD_TAGS_AS_LABELS)
