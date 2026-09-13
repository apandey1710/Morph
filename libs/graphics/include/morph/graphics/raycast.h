#pragma once

#include <raylib.h>

#include <cmath>
#include <limits>
#include <optional>

namespace morph::graphics
{
    enum class Side
    {
        X,
        Y
    };

    struct RayHit
    {
        int cellX;
        int cellY;
        Side side;
        double t;
        Vector2 position;
    };

// Template definitions must be visible to callers.
template<int Height, int Width>
std::optional<RayHit> castRay(const int (&map) [Height][Width], const Vector2 origin, Vector2 direction)
{

    if (!std::isfinite(origin.x) || !std::isfinite(origin.y) || !std::isfinite(direction.x) || !std::isfinite(direction.y)  ||
        (direction.x == 0.0f && direction.y == 0.0f) || origin.x < 0 || origin.y < 0 || origin.x >= Width || origin.y >= Height)
    {
        return std::nullopt;
    }

    const int stepX = direction.x < 0 ? -1 : 1;
    const int stepY = direction.y < 0 ? -1 : 1;

    int cellX = static_cast<unsigned int>(std::floor(origin.x));
    int cellY = static_cast<unsigned int>(std::floor(origin.y));
    if (map[cellY][cellX] != 0)
    {
        return std::nullopt;
    }

    const double infinity = std::numeric_limits<double>::infinity();
    double deltaX =  direction.x == 0 ? infinity : std::abs(1/direction.x);
    double deltaY =  direction.y == 0 ? infinity : std::abs(1/direction.y);

    double nextX = direction.x == 0 ? infinity : (direction.x < 0.0 ? origin.x - cellX : cellX + 1.0 - origin.x) * deltaX;
    double nextY = direction.y == 0 ? infinity : (direction.y < 0.0 ? origin.y - cellY : cellY + 1.0 - origin.y) * deltaY;

    for (auto crossing = 0; crossing < Width + Height; ++crossing)
    {
        Side s;
        double t;

        if (nextX <= nextY)
        {
            t = nextX;
            s = Side::X;
            cellX += stepX;
            nextX += deltaX;
        }
        else
        {
            t = nextY;
            s = Side::Y;
            cellY += stepY;
            nextY += deltaY;
        }

        if (cellX < 0 || cellX >= Width || cellY < 0 || cellY >= Height)
        {
            return std::nullopt;
        }

        if (map[cellY][cellX] != 0)
        {
            return RayHit{
                cellX,
                cellY,
                s,
                t,
                Vector2(
                    static_cast<float>(origin.x + t * direction.x),
                    static_cast<float>(origin.y + t * direction.y)
                    ),
            };
        }


    }
    return std::nullopt;
}

}
