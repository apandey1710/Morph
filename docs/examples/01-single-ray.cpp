#include "scene.hpp"

int main()
{
    InitWindow(960, 540, "Morph / 01 / One ray");
    if (!IsWindowReady())
    {
        return 1;
    }
    SetTargetFPS(60);
    float angle = 0.0f;
    while (!WindowShouldClose())
    {
        const float turn = GetFrameTime() * 1.2f; // Radians per second.
        if (IsKeyDown(KEY_LEFT))
        {
            angle -= turn;
        }
        if (IsKeyDown(KEY_RIGHT))
        {
            angle += turn;
        }
        const Vector2 direction = forwardAt(angle);
        const auto hit = castRay(level, playerPosition, direction);
        BeginDrawing();
        ClearBackground(Color{14, 21, 26, 255});
        drawMap();
        if (hit)
        {
            drawHit(*hit, hit->side == Side::X ? SKYBLUE : ORANGE);
        }
        drawPlayer(direction);
        DrawText("ONE RAY", 440, 50, 28, RAYWHITE);
        DrawText("Left / Right: rotate", 440, 100, 20, LIGHTGRAY);
        DrawText("Blue: X crossing\nOrange: Y crossing", 440, 150, 20, LIGHTGRAY);
        EndDrawing();
    }
    CloseWindow();
}
