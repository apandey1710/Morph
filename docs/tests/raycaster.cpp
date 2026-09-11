#include "../examples/raycaster.hpp"
#include <cassert>
#include <iostream>

bool close(double a, double b)
{
    return std::abs(a - b) < 0.00001;
}

int main()
{
    const Vector2 p{2.5f, 1.5f};
    const auto east = castRay(level, p, Vector2{1, 0});
    assert(east && east->cellX == 7 && east->cellY == 1);
    assert(east->side == Side::X && close(east->t, 4.5));
    assert(close(east->position.x, 7) && close(east->position.y, 1.5));
    const auto north = castRay(level, p, Vector2{0, -1});
    assert(north && north->side == Side::Y && close(north->t, 0.5));
    const auto doubled = castRay(level, p, Vector2{2, 0});
    assert(doubled && close(doubled->t, 2.25));
    assert(close(doubled->position.x, east->position.x));
    const auto diagonal = castRay(level, p, Vector2{0.8f, 0.6f});
    assert(diagonal && diagonal->cellX == 3 && diagonal->cellY == 2);
    assert(diagonal->side == Side::Y && close(diagonal->t, 5.0/6.0));
    assert(close(diagonal->position.x, 19.0/6.0));
    assert(!castRay(level, p, Vector2{0, 0}));
    assert(!castRay(level, Vector2{-0.1f, 1.5f}, Vector2{1, 0}));
    assert(!castRay(level, Vector2{0.5f, 0.5f}, Vector2{1, 0}));
    assert(!castRay(level, p, Vector2{std::numeric_limits<float>::infinity(), 0}));
    assert(!castRay(level, Vector2{std::numeric_limits<float>::quiet_NaN(), 1}, Vector2{1, 0}));
    const int openMap[3][5] = {};
    assert(!castRay(openMap, Vector2{2.5f, 1.5f}, Vector2{1, 0}));
    const int corner[3][3] = {{0,1,0}, {0,1,0}, {0,0,0}};
    const auto tie = castRay(corner, Vector2{0.5f,0.5f}, Vector2{1,1});
    assert(tie && tie->cellX == 1 && tie->cellY == 0 && tie->side == Side::X);
    const auto boundary = castRay(level, Vector2{1,1.5f}, Vector2{-1,0});
    assert(boundary && boundary->t == 0 && boundary->cellX == 0);
    // All rays against this front-facing wall must have the same camera depth.
    const int room[5][5] = {{1,1,1,1,1},{1,0,0,0,1},{1,0,0,0,1},{1,0,0,0,1},{1,1,1,1,1}};
    for (int i = 0; i < 51; ++i)
    {
        const float u = 2.0f * (i + 0.5f) / 51 - 1;
        const Vector2 ray{1, u * 0.5f};
        const auto hit = castRay(room, Vector2{2.5f,2.5f}, ray);
        assert(hit && hit->side == Side::X && close(hit->t, 1.5));
        assert(close(hit->position.x - 2.5, hit->t));
    }
    std::cout << "DDA and camera-depth checks passed\n";
}
