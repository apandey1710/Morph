#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include <raycast.h>

using morph::graphics::castRay;
using morph::math::Vec2;
using morph::graphics::Side;
using Catch::Matchers::WithinAbs;

namespace
{
    // 5x5 room: walls on the border, open 3x3 interior.
    // Indexed map[y][x]; world units are cells; +x right, +y down.
constexpr int room[5][5] = {
    {1, 1, 1, 1, 1},
    {1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1},
    {1, 1, 1, 1, 1},
};
}

TEST_CASE("castRay hits the east wall from the room centre", "[raycast]")
{
    const auto hit = castRay(room, Vec2{2.5f, 2.5f}, Vec2{1.0f, 0.0f});

    REQUIRE(hit.has_value());                 // stop here if nullopt: *hit would be UB
    CHECK(hit->cellX == 4);
    CHECK(hit->cellY == 2);
    CHECK(hit->side == Side::X);
    CHECK_THAT(hit->t, WithinAbs(1.5, 1e-9));
    CHECK_THAT(hit->position.x, WithinAbs(4.0, 1e-6));   // float → looser tolerance
    CHECK_THAT(hit->position.y, WithinAbs(2.5, 1e-6));
}

TEST_CASE("castRay reports the near face of a wall to the west", "[raycast]")
{
    // Moving -x: wall cell is x=0, but the face we hit is its east edge, x=1.
    const auto hit = castRay(room, Vec2{2.5f, 2.5f}, Vec2{-1.0f, 0.0f});

    REQUIRE(hit.has_value());
    CHECK(hit->cellX == 0);
    CHECK(hit->side == Side::X);
    CHECK_THAT(hit->position.x, WithinAbs(1.0, 1e-6));
}

TEST_CASE("castRay t is measured in units of the direction vector", "[raycast]")
{
    // Same ray as the first test, but direction has length 2.
    // Distance to the wall is still 1.5, so t = 1.5 / 2 = 0.75.
    const auto hit = castRay(room, Vec2{2.5f, 2.5f}, Vec2{2.0f, 0.0f});

    REQUIRE(hit.has_value());
    CHECK_THAT(hit->t, WithinAbs(0.75, 1e-9));
    CHECK_THAT(hit->position.x, WithinAbs(4.0, 1e-6));
}

TEST_CASE("castRay prefers the X side when crossings tie", "[raycast][edge]")
{
    // Diagonal from a cell centre: nextX == nextY at every step.
    // `nextX <= nextY` means X wins ties, so we enter (4,3) through its west face.
    const auto hit = castRay(room, Vec2{2.5f, 2.5f}, Vec2{1.0f, 1.0f});

    REQUIRE(hit.has_value());
    CHECK(hit->cellX == 4);
    CHECK(hit->cellY == 3);
    CHECK(hit->side == Side::X);
    CHECK_THAT(hit->t, WithinAbs(1.5, 1e-9));
}
