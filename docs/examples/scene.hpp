#pragma once

#include "raycaster.hpp"

constexpr float pixelsPerCell = 48.0f;
constexpr int offsetX = 24;
constexpr int offsetY = 24;
constexpr Vector2 playerPosition{2.5f, 1.5f};
constexpr float pi = 3.14159265358979323846f;

inline Vector2 toScreen(Vector2 world)
{
    return Vector2{offsetX + world.x * pixelsPerCell,
                   offsetY + world.y * pixelsPerCell};
}

inline void drawMap()
{
    for (int y = 0; y < mapHeight; ++y)
    {
        for (int x = 0; x < mapWidth; ++x)
        {
            DrawRectangle(offsetX + x * static_cast<int>(pixelsPerCell),
                          offsetY + y * static_cast<int>(pixelsPerCell),
                          static_cast<int>(pixelsPerCell) - 1,
                          static_cast<int>(pixelsPerCell) - 1,
                          level[y][x] == 0 ? Color{27, 37, 43, 255} :
                                            Color{75, 94, 102, 255});
        }
    }
}

inline void drawPlayer(Vector2 forward)
{
    DrawLineV(toScreen(playerPosition),
              toScreen(Vector2{playerPosition.x + forward.x * 0.75f,
                               playerPosition.y + forward.y * 0.75f}), WHITE);
    DrawCircleV(toScreen(playerPosition), 5.0f, WHITE);
}

inline void drawHit(const RayHit& hit, Color color)
{
    DrawLineV(toScreen(playerPosition), toScreen(hit.position), color);
    DrawCircleV(toScreen(hit.position), 3.0f, color);
}

// Unit forward vector; positive angles turn clockwise in our y-down world.
inline Vector2 forwardAt(float angle)
{
    return Vector2{std::cos(angle), std::sin(angle)};
}

// Perpendicular camera plane. Its half-width at depth 1 sets horizontal FOV.
inline Vector2 planeFor(Vector2 forward, float fovRadians)
{
    const float halfWidth = std::tan(fovRadians * 0.5f);
    return Vector2{-forward.y * halfWidth, forward.x * halfWidth};
}

inline Vector2 cameraRay(Vector2 forward, Vector2 plane, float u)
{
    return Vector2{forward.x + u * plane.x, forward.y + u * plane.y};
}
