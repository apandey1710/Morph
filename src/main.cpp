#include <raylib.h>

constexpr unsigned int mapWidth = 8;
constexpr unsigned int mapHeight = 8;
constexpr unsigned int pixelsPerCell = 48;
constexpr int offsetX = 24;
constexpr int offsetY = 24;

constexpr Vector2 playerPosition{2.5f, 1.5f};
constexpr Vector2 playerScreen
{
    offsetX + playerPosition.x * pixelsPerCell,
    offsetY + playerPosition.y * pixelsPerCell
};

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
                DrawCircleV(playerScreen, 5.0f, BLUE);
                DrawLineV(
                    playerScreen,
                    Vector2{playerScreen.x + 1.9 * pixelsPerCell, playerScreen.y},
                    RED
                    );
            }
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

