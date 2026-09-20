#include <raycast.h>

#include <raylib.h>

constexpr unsigned int mapWidth = 8;
constexpr unsigned int mapHeight = 8;
constexpr unsigned int pixelsPerCell = 48;
constexpr int offsetX = 24;
constexpr int offsetY = 24;

constexpr Vector2 playerPosition{2.7f, 1.3f};

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

        for (auto y = 0; y < mapWidth; y++)
        {
            for (auto x = 0; x < mapHeight; x++)
            {
                const Color color = level[y][x] == 0 ? LIGHTGRAY : DARKGRAY;
                DrawRectangle(
                    offsetX + x * pixelsPerCell,
                    offsetY + y * pixelsPerCell,
                    pixelsPerCell - 1,
                    pixelsPerCell - 1,
                    color);
                Vector2 playerScreen  = toScreen(playerPosition);
                DrawCircleV(playerScreen, 5.0f, BLUE);
            }
        }

        constexpr Vector2 direction = {2.0f, 0.64f};

        const auto hit = morph::graphics::castRay(level, playerPosition, direction);
        if (hit)
        {
            const Color color = hit->side == morph::graphics::Side::X ? SKYBLUE : ORANGE;
            DrawLineV(toScreen(playerPosition), toScreen(hit->position), color);
            DrawCircleV(toScreen(hit->position), 3.0f, color);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
