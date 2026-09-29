#pragma once

#include <cstdint>
#include <cstddef>
#include <optional>
#include <vector>
#include <span>
#include <array>

namespace morph::util
{
    struct Map
    {
        std::uint32_t width = 0;
        std::uint32_t height = 0;
        std::vector<std::uint8_t> cells;

        std::optional<std::uint8_t> cell(std::uint32_t x, std::uint32_t y) const;
    };

    std::optional<Map> decodeMap(std::span<const std::byte> bytes);
    std::optional<std::vector<std::byte>> encodeMap(const Map& map);
}

