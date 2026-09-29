#pragma once

namespace morph::math
{
    // A point or direction on the horizontal world plane.
    // Units: world units (1.0 = one map cell); +x right, +y down.
    struct Vec2
    {
        float x = 0.0f;
        float y = 0.0f;
    };
}

