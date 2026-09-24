include(CTest)

add_executable(morph_io_tests io_tests.cpp)
target_link_libraries(morph_io_tests PRIVATE morph::io)

add_test(NAME morph_io_tests COMMAND morph_io_tests)