#include "scene.hpp"

int main()
{
    InitWindow(960, 540, "Morph / 02 / Ray fan");
    if (!IsWindowReady())
    {
        return 1;
    }
    SetTargetFPS(60);
    float angle = 0.0f;
    constexpr float fovRadians = 60.0f * pi / 180.0f;
    constexpr int rayCount = 31;
    while (!WindowShouldClose())
    {
        const float turn = GetFrameTime() * 1.2f;
        if (IsKeyDown(KEY_LEFT))
        {
            angle -= turn;
        }
        if (IsKeyDown(KEY_RIGHT))
        {
            angle += turn;
        }
        const Vector2 forward = forwardAt(angle);
        const Vector2 plane = planeFor(forward, fovRadians);
        BeginDrawing();
        ClearBackground(Color{14, 21, 26, 255});
        drawMap();
        for (int i = 0; i < rayCount; ++i)
        {
            const float u = 2.0f * (static_cast<float>(i) + 0.5f) / rayCount - 1.0f;
            const auto hit = castRay(level, playerPosition, cameraRay(forward, plane, u));
            if (hit)
            {
                drawHit(*hit, hit->side == Side::X ? SKYBLUE : ORANGE);
            }
        }
        drawPlayer(forward);
        DrawText("A FAN OF RAYS", 440, 50, 28, RAYWHITE);
        DrawText("Left / Right: rotate\n60 degree horizontal FOV\n31 camera-plane samples", 440, 100, 20, LIGHTGRAY);
        EndDrawing();
    }
    CloseWindow();
}
