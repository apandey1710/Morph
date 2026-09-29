#include <util.hpp>
#include <algorithm>

namespace
{

void appendU32(std::vector<std::byte> &out, std::uint32_t value)
{
    // Little endian encoding.
    for (std::size_t i = 0; i < 4; ++i)
    {
        out.push_back(static_cast<std::byte>( (value >> (8 * i)) & 0xFF ));
    }
}

std::uint32_t readU32(std::span<const std::byte> in, std::size_t offset)
{
    std::uint32_t value = 0;
    for (std::size_t i = 0; i < 4; ++i)
    {
        value |= std::to_integer<std::uint32_t>(in[offset + i]) << (8 * i);
    }

    return value;
}

}


namespace morph::util
{

constexpr std::array<std::byte, 4> kMagic{std::byte{'M'}, std::byte{'R'}, std::byte{'P'}, std::byte{'H'}};
constexpr std::size_t kWidthOffset  = 4;
constexpr std::size_t kHeightOffset = 8;
constexpr std::size_t kCellsOffset  = 12;

std::optional<std::uint8_t> Map::cell(std::uint32_t x, std::uint32_t y) const
{
    if (x >= width || y >= height)
    {
        return std::nullopt;
    }

    return cells[x + width * y];
}

std::optional<Map> decodeMap(std::span<const std::byte> bytes)
{

    if (bytes.size() < kCellsOffset)
    {
        return std::nullopt;
    }

    if (!std::equal(kMagic.begin(), kMagic.end(), bytes.begin()))
    {
        return std::nullopt;
    }

    const std::uint32_t width = readU32(bytes, kWidthOffset);
    const std::uint32_t height = readU32(bytes, kHeightOffset);
    const std::uint64_t cellCount = std::uint64_t{width} * height;

    if (width == 0 || height == 0 || bytes.size() - kCellsOffset != cellCount)
    {
        return std::nullopt;
    }
    
    morph::util::Map map{0, 0, {}};
    map.width = width;
    map.height = height;
    map.cells.reserve(map.width * map.height);

    for (std::byte b : bytes.subspan(kCellsOffset))
    {
        map.cells.push_back(std::to_integer<std::uint8_t>(b));
    }

    return map;
}
    
std::optional<std::vector<std::byte>> encodeMap(const Map &map)
{
    if (map.width == 0 || map.height == 0 || map.cells.size() != std::uint64_t(map.width) * map.height)
    {
        return std::nullopt;
    }
    
    std::vector<std::byte> out;
    out.reserve(kCellsOffset + map.cells.size());
    out.insert(out.end(), kMagic.begin(), kMagic.end());
    appendU32(out, map.width);
    appendU32(out, map.height);
    for (std::uint8_t c : map.cells)
    {
        out.push_back(std::byte(c));
    }

    return out;
}

} // namespace morph::util

