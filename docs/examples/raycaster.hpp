#pragma once

#include <raylib.h>
#include <cmath>
#include <limits>
#include <optional>

constexpr int mapWidth = 8;
constexpr int mapHeight = 8;
constexpr int level[mapHeight][mapWidth] = {
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 1, 0, 0, 0, 1},
    {1, 0, 0, 1, 0, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1},
};

enum class Side
{
    X, // Crossed a vertical grid line: the cell's x index changed.
    Y  // Crossed a horizontal grid line: the cell's y index changed.
};

struct RayHit
{
    int cellX;
    int cellY;
    Side side;
    double t; // Parameter in origin + t * direction, not always ray length.
    Vector2 position;
};

// Accept a map by reference so dimensions are inferred, including non-square maps.
// Empty origin required. Invalid rays, solid origins, and map exits return nullopt.
template <int Height, int Width>
std::optional<RayHit> castRay(const int (&map)[Height][Width],
                              Vector2 origin, Vector2 direction)
{
    if (!std::isfinite(origin.x) || !std::isfinite(origin.y) ||
        !std::isfinite(direction.x) || !std::isfinite(direction.y) ||
        (direction.x == 0.0f && direction.y == 0.0f) ||
        origin.x < 0.0f || origin.x >= Width ||
        origin.y < 0.0f || origin.y >= Height)
    {
        return std::nullopt;
    }

    int cellX = static_cast<int>(std::floor(origin.x));
    int cellY = static_cast<int>(std::floor(origin.y));
    if (map[cellY][cellX] != 0)
    {
        return std::nullopt;
    }

    const double infinity = std::numeric_limits<double>::infinity();
    const int stepX = direction.x < 0.0f ? -1 : 1;
    const int stepY = direction.y < 0.0f ? -1 : 1;
    const double deltaX = direction.x == 0.0f ? infinity :
        std::abs(1.0 / direction.x);
    const double deltaY = direction.y == 0.0f ? infinity :
        std::abs(1.0 / direction.y);

    // Guard zero components explicitly: 0 * infinity would produce NaN.
    double nextX = direction.x == 0.0f ? infinity :
        (direction.x < 0.0f ? origin.x - cellX : cellX + 1.0 - origin.x) * deltaX;
    double nextY = direction.y == 0.0f ? infinity :
        (direction.y < 0.0f ? origin.y - cellY : cellY + 1.0 - origin.y) * deltaY;

    // Each step changes one cell index monotonically; this bound covers map exit.
    for (int crossing = 0; crossing < Width + Height; ++crossing)
    {
        double t;
        Side side;
        if (nextX <= nextY) // Exact corner ties deliberately visit X first.
        {
            t = nextX;
            nextX += deltaX;
            cellX += stepX;
            side = Side::X;
        }
        else
        {
            t = nextY;
            nextY += deltaY;
            cellY += stepY;
            side = Side::Y;
        }

        if (cellX < 0 || cellX >= Width || cellY < 0 || cellY >= Height)
        {
            return std::nullopt;
        }
        if (map[cellY][cellX] != 0)
        {
            return RayHit{cellX, cellY, side, t,
                Vector2{static_cast<float>(origin.x + t * direction.x),
                        static_cast<float>(origin.y + t * direction.y)}};
        }
    }
    return std::nullopt;
}
