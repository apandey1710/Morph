#include "scene.hpp"
#include <algorithm>

int main()
{
    constexpr int screenWidth = 960;
    constexpr int screenHeight = 540;
    InitWindow(screenWidth, screenHeight, "Morph / 03 / Wall strips");
    if (!IsWindowReady())
    {
        return 1;
    }
    SetTargetFPS(60);
    float angle = 0.0f;
    constexpr float fovRadians = 60.0f * pi / 180.0f;
    const double focalPixels = (screenWidth * 0.5) / std::tan(fovRadians * 0.5);
    constexpr double wallHeight = 1.0; // World units; eye height is half this.
    constexpr double nearDepth = 0.001; // 1/1000 cell; prevents division by zero.
    bool useRayLength = false;
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
        if (IsKeyPressed(KEY_F))
        {
            useRayLength = !useRayLength;
        }
        const Vector2 forward = forwardAt(angle);
        const Vector2 plane = planeFor(forward, fovRadians);
        BeginDrawing();
        ClearBackground(Color{22, 31, 42, 255});
        DrawRectangle(0, screenHeight / 2, screenWidth, screenHeight / 2,
                      Color{34, 41, 43, 255});
        for (int x = 0; x < screenWidth; ++x)
        {
            const float u = 2.0f * (static_cast<float>(x) + 0.5f) / screenWidth - 1.0f;
            const Vector2 ray = cameraRay(forward, plane, u); // Do not normalize.
            const auto hit = castRay(level, playerPosition, ray);
            if (!hit)
            {
                continue;
            }
            double depth = hit->t; // forward is unit and plane is perpendicular.
            if (useRayLength) // Deliberately incorrect: inspect fisheye distortion.
            {
                depth *= std::hypot(ray.x, ray.y);
            }
            depth = std::max(depth, nearDepth);
            const double height = wallHeight * focalPixels / depth;
            // Clip in floating point before converting to pixel integers.
            const double top = std::clamp((screenHeight - height) * 0.5, 0.0,
                                          static_cast<double>(screenHeight));
            const double bottom = std::clamp((screenHeight + height) * 0.5, 0.0,
                                             static_cast<double>(screenHeight));
            const int yStart = static_cast<int>(std::floor(top));
            const int yEnd = static_cast<int>(std::ceil(bottom));
            DrawRectangle(x, yStart, 1, yEnd - yStart,
                          hit->side == Side::X ? Color{111, 207, 192, 255} :
                                                Color{58, 130, 136, 255});
        }
        DrawText("Left / Right: rotate | F: compare depth", 20, 20, 20, WHITE);
        DrawText(useRayLength ? "Ray length (incorrect)" : "Camera depth (correct)",
                 20, 48, 20, WHITE);
        EndDrawing();
    }
    CloseWindow();
}
