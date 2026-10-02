# Running Morph's tests

Morph's tests use two tools with separate jobs:

- **Catch2** (v3.8.1) runs *inside* each test executable. It provides
  `TEST_CASE`, `CHECK`, and `REQUIRE`, and when a check fails it prints the
  values that were compared.
- **CTest** (part of CMake) runs *outside* the executables. It finds every test,
  runs it, and reports which passed, failed, or were skipped.

Each library has its own test executable, so each links only the library it
tests:

```text
tests/io_tests.cpp       ->  morph_io_tests       links morph::io
tests/util_tests.cpp     ->  morph_util_tests     links morph::util
tests/raycast_tests.cpp  ->  morph_raycast_tests  links morph::graphics (and morph::math)
```

## 1. Build and run everything

From the repository root:

```sh
cmake --preset clang-debug
cmake --build --preset clang-debug
ctest --preset clang-debug
```

Tests are on by default. The first configure downloads Catch2 with CMake
FetchContent and verifies the archive with SHA-256, the same way raylib is
fetched.

The test preset turns on CTest's `--output-on-failure`, which prints Catch2's
report for any test that fails. Without it, CTest only shows `Failed` and you
have to rerun the test to find out why.

Right now each file contains a single placeholder that calls `SKIP`, so a clean
run looks like this:

```text
1/3 Test #1: io tests are not written yet ........***Skipped   0.00 sec
2/3 Test #2: util tests are not written yet ......***Skipped   0.00 sec
3/3 Test #3: raycast tests are not written yet ...***Skipped   0.00 sec

100% tests passed out of 3
```

A skipped test counts as "not failed", not as "tested". The placeholders use
`SKIP` so they stay visible until real tests replace them.

For multi-configuration generators such as Visual Studio, also tell CTest which
configuration to run: `ctest --test-dir build -C Debug --output-on-failure`.

## 2. How CTest learns about individual tests

CTest doesn't read C++. `catch_discover_tests` in `tests/CMakeLists.txt` runs
each test executable after it is built, asks Catch2 for its list of
`TEST_CASE`s, and registers each one as a separate CTest test. The test case
name becomes the CTest test name, and its tags (such as `[util]`) become CTest
labels.

Two consequences:

- **After adding a `TEST_CASE`, rebuild before running `ctest`.** Discovery
  happens at build time, so CTest can't see a new test until the executable has
  been rebuilt.
- **Test files must not define `main()`.** `Catch2::Catch2WithMain` supplies
  `main`. A file with its own `main` still links, but it no longer answers
  Catch2's listing request, so discovery fails and the build stops.

## 3. Choosing which tests to run

```sh
ctest --preset clang-debug -N                # list tests without running them
ctest --preset clang-debug -R util           # names matching a regular expression
ctest --preset clang-debug -L raycast        # tests with a label (a Catch2 tag)
ctest --preset clang-debug --print-labels    # list every label
ctest --preset clang-debug --rerun-failed
ctest --preset clang-debug -j 8              # run tests in parallel
```

`-R` and `-L` both take regular expressions, so `-L "util|raycast"` selects
both groups.

## 4. Running a test executable directly

The executables are in `build/clang-debug/tests/`. Running one directly skips
CTest and gives you Catch2's own options, which help while you're working on a
single test:

```sh
./build/clang-debug/tests/morph_util_tests                # run every test case in the file
./build/clang-debug/tests/morph_util_tests "[util]"       # only test cases with a tag
./build/clang-debug/tests/morph_util_tests "cell*"        # test cases whose names match a wildcard
./build/clang-debug/tests/morph_util_tests --list-tests   # list test cases and their tags
./build/clang-debug/tests/morph_util_tests --success      # also print checks that passed
```

This is also the easiest way to run one test under a debugger.

## 5. Reading a failure

Suppose `tests/util_tests.cpp` contains this test for a 3-by-2 map, stored row
by row (`cells[y * width + x]`):

```cpp
#include <catch2/catch_test_macros.hpp>

#include <util.hpp>

TEST_CASE("cell() indexes row-major on a non-square map", "[util][map]")
{
    morph::util::Map map{3, 2, {0, 1, 2, 3, 4, 5}};

    const auto cell = map.cell(1, 0);
    REQUIRE(cell.has_value());
    CHECK(*cell == 1);
}
```

Against the current `Map::cell`, which computes `x * width + y`, CTest reports
(Catch2's banner lines trimmed):

```text
2/3 Test #2: cell() indexes row-major on a non-square map ...***Failed    0.00 sec
-------------------------------------------------------------------------------
cell() indexes row-major on a non-square map
-------------------------------------------------------------------------------
tests/util_tests.cpp:5
...............................................................................

tests/util_tests.cpp:11: FAILED:
  CHECK( *cell == 1 )
with expansion:
  3 == 1
```

Read it from the bottom up:

- `CHECK( *cell == 1 )` is the expression as written in the source.
- `with expansion: 3 == 1` shows the actual values. Cell `(1, 0)` returned the
  value stored at index `1 * 3 + 0 = 3` instead of index `0 * 3 + 1 = 1`, so the
  x and y axes are swapped.
- `tests/util_tests.cpp:11` is the line of the failed check. The `:5` above it
  is the line where the test case starts.

When any test fails, `ctest` exits with a non-zero status (8 in this case). A
run where tests are only skipped exits with 0.

### `REQUIRE` or `CHECK`?

- `REQUIRE` stops the test case at the first failure. Use it when the rest of
  the test would be meaningless or unsafe, such as dereferencing an empty
  `std::optional`.
- `CHECK` records the failure and keeps going. Use it for independent facts, so
  one run reports every value that is wrong.

### Compare values, not optionals

Catch2 can't print `std::optional` by default. `CHECK(map.cell(1, 0) == 1)`
still fails correctly, but its expansion shows `{?} == 1`, which hides the
actual value. Check `has_value()` with `REQUIRE`, then compare the contained
value as above.

## 6. Adding tests

- **For an existing library:** add a `TEST_CASE` to the library's file in
  `tests/`, give it a tag, rebuild, and run `ctest`. Replace the file's `SKIP`
  placeholder once it has a real test.
- **For a new executable:** add a source file and three lines to
  `tests/CMakeLists.txt`, following the existing pattern:

  ```cmake
  add_executable(morph_math_tests math_tests.cpp)
  target_link_libraries(morph_math_tests PRIVATE morph::math Catch2::Catch2WithMain)
  catch_discover_tests(morph_math_tests ADD_TAGS_AS_LABELS)
  ```

Tests should call pure functions and compare exact, hand-worked results, such as
a ray's hit cell, side, and distance, or a map lookup. They must not open a
raylib window: they need to run without a display.

For floating-point results, compare within a tolerance instead of using `==`:

```cpp
#include <catch2/matchers/catch_matchers_floating_point.hpp>

CHECK_THAT(hit->t, Catch::Matchers::WithinAbs(2.5, 1e-9));
```

## 7. Troubleshooting

| Symptom | Cause and fix |
|---|---|
| `Total Tests: 0` | Tests are off. Reconfigure with `-DBUILD_TESTING=ON`, or check that you ran `ctest` against the right build directory. |
| A test named `morph_..._tests_NOT_BUILT-...` | The executable didn't build, or discovery failed (often a stray `main()`). Build again and read the first error. |
| A new `TEST_CASE` doesn't appear | Rebuild before running `ctest`. Discovery happens at build time. |
| A failure shows `{?}` | Catch2 can't print that type. Compare a printable value instead (see section 5). |
| First configure fails while downloading Catch2 | You're offline. Point CMake at an existing copy of Catch2 v3.8.1 with `-DFETCHCONTENT_SOURCE_DIR_CATCH2=/path/to/Catch2`. |

To build the game without tests (skips downloading Catch2):

```sh
cmake -S . -B build -DBUILD_TESTING=OFF
```
