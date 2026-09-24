#pragma once

#include <cstdint>
#include <optional>
#include <vector>

namespace morph::util
{
// Future shared utility declarations belong here.
    struct Map
    {
        std::uint32_t width;
        std::uint32_t height;
        std::vector<std::uint8_t> cells;

        std::optional<std::uint8_t> cell(std::uint32_t x, std::uint32_t y);
    };

    std::optional<Map> decodeMap(const std::vector<std::byte>& bytes);
    std::optional<std::vector<std::byte>> encodeMap(const Map& bytes);
}

