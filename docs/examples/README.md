# Morph testing checkpoint

Requires CMake 3.25+, a C++20 compiler, and a build tool. First configuration
requires network access to download pinned Catch2 3.8.1. No raylib required.
From this directory:

    cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
    cmake --build build --parallel 2
    ctest --test-dir build --output-on-failure

For Visual Studio or other multi-configuration generators, add --config Debug
to the build and -C Debug to ctest. Eight named test cases are discovered;
the empty-write case has two SECTION paths. No custom main is needed.

io.cpp/io.hpp are a teaching snapshot of Morph's IO implementation with explicit
standard-library includes. It retains the current limitation: writeFile does
not report errors after opening. The tests cover normal IO and open failures,
not disk-full, concurrent modification, or durability. Do not copy the snapshot
over your project's implementation; integrate the tests with morph::io instead.
