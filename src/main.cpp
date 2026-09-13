#include <morph/graphics/raycast.h>

#include <raylib.h>

constexpr unsigned int mapWidth = 8;
constexpr unsigned int mapHeight = 8;
constexpr unsigned int pixelsPerCell = 48;
constexpr int offsetX = 24;
constexpr int offsetY = 24;

constexpr Vector2 playerPosition{2.5f, 1.5f};

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

Vector2 toScreen(Vector2 world)
{
    return Vector2 {
        offsetX + world.x * pixelsPerCell,
        offsetY + world.y * pixelsPerCell
    };
}



int main()
{
    constexpr int screenWidth = 960;
    constexpr int screenHeight = 540;

    InitWindow(screenWidth, screenHeight, "Morph");
    if (!IsWindowReady())
    {
        return 1;
    }

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        constexpr Vector2 direction{0.0f, 1.0f};
        const auto hit = morph::graphics::castRay(level, playerPosition, direction);
        if (hit)
        {
            const Color color = hit->side == morph::graphics::Side::X ? SKYBLUE : ORANGE;
            DrawLineV(toScreen(playerPosition), toScreen(hit->position), color);
            DrawCircleV(toScreen(hit->position), 3.0f, color);
        }
        DrawCircleV(toScreen(playerPosition), 5.0f, RED);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
