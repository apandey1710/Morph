#include <util.hpp>

namespace morph::util
{
// Future non-inline utility implementations belong here.
std::optional<std::uint8_t> Map::cell(std::uint32_t x, std::uint32_t y)
{
    if (x >= width || x < 0 || y >= height || y < 0)
    {
        return std::nullopt;
    }

    return cells[x * width + y];
}

std::optional<Map> decodeMap(const std::vector<std::byte> &bytes)
{
    return std::nullopt;
}

std::optional<std::vector<std::byte>> encodeMap(const Map &bytes)
{
    return std::nullopt;
}

}

