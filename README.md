# Morph

A C++ graphics learning project building toward a Wolfenstein-style raycaster.
The initial application opens a raylib window and draws "Hello, world!".

## Build and run

Requires CMake 3.25 or newer, a C compiler, a C++20 compiler, and a native build
tool such as Make, Ninja, or Visual Studio. The first configure downloads raylib
6.0 using CMake FetchContent; its archive is verified with SHA-256.

On Linux, raylib also needs OpenGL and window-system development packages.
See the [official Linux build instructions](https://github.com/raysan5/raylib/wiki/Working-on-GNU-Linux)
for your distribution. This setup uses raylib's default desktop backend (X11 on
Linux, including XWayland on Wayland desktops).

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --parallel
./build/morph
```

For multi-configuration generators such as Visual Studio, build with
`cmake --build build --config Debug`; the executable is under `build/Debug/`
(`morph.exe` on Windows). `CMAKE_BUILD_TYPE` applies to single-configuration
generators.

The window displays centered text. Press Escape or close the window to exit.

## Level and coordinate conventions

These conventions define the initial grid-based level model; level rendering
is not implemented yet.

- Store cells as `level[y][x]`: row first, then column. `0` means empty space
  and `1` means a wall.
- Each cell occupies one square world unit. Wall cells represent solid
  footprints with a common vertical height; empty cells are open space between
  the floor and ceiling.
- The world origin `(0, 0)` is the top-left corner of the map. On the overhead
  view, `+x` points right and `+y` points down. These axes describe the horizontal
  world plane, not vertical height.
- Store world positions as floating-point coordinates and map indices as
  integers. Keep player position separate from cell contents.
- Cell `(x, y)` covers `[x, x + 1)` horizontally and `[y, y + 1)` vertically.
  Its center is `(x + 0.5, y + 0.5)`. For example, player position `(2.5, 1.5)`
  is the center of cell `(2, 1)`.
- Convert a world position to a cell with `floor`, then check that
  `0 <= x < mapWidth` and `0 <= y < mapHeight` before indexing. Integer casts
  truncate toward zero, so they do not give the correct cell for negative
  positions. Enclosing walls do not replace bounds checks.
- Direction `(1, 0)` points east (right); `(0, 1)` points south (down).

World units and screen pixels are separate. For an overhead map, use a positive
scale `pixelsPerCell` and a screen offset `(offsetX, offsetY)`, measured in
pixels:

```text
screenX = offsetX + worldX * pixelsPerCell
screenY = offsetY + worldY * pixelsPerCell
```

Screen coordinates start at the top-left, with `+x` right and `+y` down.
Changing the drawing scale changes the map's onscreen size, not its world
dimensions. Any gaps drawn between cells are visual only; world cells touch.

For first-person projection, distinguish distance traveled along a ray from
perpendicular camera depth (distance along the camera's forward axis). Wall
projection uses the latter to avoid fisheye distortion. A ray parameter is a
physical distance only when the ray direction has unit length.

Rendering and collision are separate: displaying a wall does not prevent the
player from moving through it.

## Layout and targets

```text
src/main.cpp                 Application entry point and drawing loop
libs/math/include Public math headers
libs/math/src                Math implementation stubs
libs/util/include Public utility headers
libs/util/src                Utility implementation stubs
libs/graphics/include Public graphics headers
libs/graphics/src            Graphics implementation stubs
libs/io/include     Public I/O headers
libs/io/src                  I/O implementation stubs
```

- `morph`: executable; links raylib and all four internal libraries.
- `morph::math`: alias for the `morph_math` static library.
- `morph::util`: alias for the `morph_util` static library.
- `morph::graphics`: alias for the `morph_graphics` static library.
- `morph::io`: alias for the `morph_io` static library; a dependency-free stub
  for file I/O, separate from map encoding and rendering.

The math, utility, and I/O libraries are intentionally empty and independent of
raylib. The graphics library depends on raylib.
Their public include paths propagate to targets that link them, allowing
`#include <math.hpp>`, `#include <util.hpp>`,
`#include <graphics.hpp>`, and `#include <io.hpp>` without
relative paths. List new implementation files explicitly in the corresponding
`CMakeLists.txt` as the project grows.

The app currently lets raylib draw text directly. A future CPU raycaster can
produce a pixel buffer and use raylib to display it; this chassis does not
implement raycasting yet.
