#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/generators/catch_generators_range.hpp>

#include <util.hpp>

#include <cstddef>
#include <cstdint>
#include <initializer_list>
#include <limits>
#include <vector>

using morph::util::Map;
using morph::util::decodeMap;
using morph::util::encodeMap;

namespace
{
// Builds bytes from integer literals so expected file contents read like a hex dump.
std::vector<std::byte> bytesOf(std::initializer_list<int> values)
{
    std::vector<std::byte> out;
    out.reserve(values.size());
    for (int v : values)
    {
        out.push_back(static_cast<std::byte>(v));
    }
    return out;
}

// Catch2 prints std::byte as {?}; comparing ints shows the actual bytes on failure.
std::vector<int> toInts(const std::vector<std::byte>& bytes)
{
    std::vector<int> out;
    out.reserve(bytes.size());
    for (std::byte b : bytes)
    {
        out.push_back(std::to_integer<int>(b));
    }
    return out;
}

// 3x3 room: walls on the border, one open cell in the middle.
// Cells are stored row by row: index = x + width * y.
Map smallRoom()
{
    return Map{3, 3, {1, 1, 1,
                      1, 0, 1,
                      1, 1, 1}};
}

// The exact file smallRoom() encodes to: 12-byte header + 9 cells = 21 bytes.
std::vector<std::byte> smallRoomFile()
{
    return bytesOf({
        0x4D, 0x52, 0x50, 0x48, // magic "MRPH"
        0x03, 0x00, 0x00, 0x00, // width  = 3, little-endian u32
        0x03, 0x00, 0x00, 0x00, // height = 3, little-endian u32
        1, 1, 1,
        1, 0, 1,
        1, 1, 1,
    });
}

// 4 wide, 2 tall, every cell different, so swapped axes or dropped cells show up.
// Includes 252..255 to catch values being treated as signed.
Map wideMap()
{
    return Map{4, 2, {0, 1, 2, 3,
                      252, 253, 254, 255}};
}
} // namespace

// ---------------------------------------------------------------- encodeMap

TEST_CASE("encodeMap writes a 3x3 map as the exact 21-byte file", "[util][encode]")
{
    const auto encoded = encodeMap(smallRoom());

    REQUIRE(encoded.has_value());
    CHECK(toInts(*encoded) == toInts(smallRoomFile()));
}

TEST_CASE("encodeMap writes width before height", "[util][encode]")
{
    const auto encoded = encodeMap(wideMap());

    REQUIRE(encoded.has_value());
    REQUIRE(encoded->size() == 12 + 8);
    const std::vector<std::byte> header(encoded->begin() + 4, encoded->begin() + 12);
    CHECK(toInts(header) == std::vector<int>{4, 0, 0, 0, 2, 0, 0, 0});
}

TEST_CASE("encodeMap stores dimensions little-endian", "[util][encode]")
{
    // 258 = 0x0102, so the low byte 0x02 must come first.
    const Map map{258, 1, std::vector<std::uint8_t>(258, 0)};

    const auto encoded = encodeMap(map);

    REQUIRE(encoded.has_value());
    const std::vector<std::byte> width(encoded->begin() + 4, encoded->begin() + 8);
    CHECK(toInts(width) == std::vector<int>{0x02, 0x01, 0x00, 0x00});
}

TEST_CASE("encodeMap rejects a map whose cell count doesn't match its size", "[util][encode]")
{
    Map map = smallRoom();

    SECTION("one cell too few")
    {
        map.cells.pop_back();
        CHECK_FALSE(encodeMap(map).has_value());
    }
    SECTION("one cell too many")
    {
        map.cells.push_back(1);
        CHECK_FALSE(encodeMap(map).has_value());
    }
}

TEST_CASE("encodeMap rejects a zero width or height", "[util][encode]")
{
    CHECK_FALSE(encodeMap(Map{0, 3, {}}).has_value());
    CHECK_FALSE(encodeMap(Map{3, 0, {}}).has_value());
}

TEST_CASE("encodeMap rejects dimensions whose product overflows 32 bits", "[util][encode][edge]")
{
    // 65536 * 65536 = 2^32, which wraps to 0 in 32-bit arithmetic and would
    // wrongly "match" an empty cell vector.
    CHECK_FALSE(encodeMap(Map{65536, 65536, {}}).has_value());
}

// ---------------------------------------------------------------- decodeMap

TEST_CASE("decodeMap reads a hand-built 3x3 file", "[util][decode]")
{
    const auto map = decodeMap(smallRoomFile());

    REQUIRE(map.has_value());
    CHECK(map->width == 3);
    CHECK(map->height == 3);
    CHECK(map->cells == smallRoom().cells);
}

TEST_CASE("decodeMap(encodeMap(m)) gives back the same map", "[util][decode][encode]")
{
    const Map original = wideMap();

    const auto encoded = encodeMap(original);
    REQUIRE(encoded.has_value());
    const auto decoded = decodeMap(*encoded);

    REQUIRE(decoded.has_value());
    CHECK(decoded->width == original.width);
    CHECK(decoded->height == original.height);
    CHECK(decoded->cells == original.cells);
}

TEST_CASE("decodeMap rejects input shorter than the 12-byte header", "[util][decode]")
{
    const auto length = GENERATE(range(std::size_t{0}, std::size_t{12}));
    CAPTURE(length);

    const auto file = smallRoomFile();
    const std::vector<std::byte> truncated(file.begin(), file.begin() + static_cast<std::ptrdiff_t>(length));

    CHECK_FALSE(decodeMap(truncated).has_value());
}

TEST_CASE("decodeMap rejects a wrong magic byte", "[util][decode]")
{
    const auto index = GENERATE(0, 1, 2, 3);
    CAPTURE(index);

    auto file = smallRoomFile();
    file[static_cast<std::size_t>(index)] ^= std::byte{0xFF};

    CHECK_FALSE(decodeMap(file).has_value());
}

TEST_CASE("decodeMap rejects a cell count that doesn't match the header", "[util][decode]")
{
    auto file = smallRoomFile();

    SECTION("one cell too few (truncated file)")
    {
        file.pop_back();
        CHECK_FALSE(decodeMap(file).has_value());
    }
    SECTION("one cell too many (trailing junk)")
    {
        file.push_back(std::byte{1});
        CHECK_FALSE(decodeMap(file).has_value());
    }
}

TEST_CASE("decodeMap rejects a zero width or height", "[util][decode]")
{
    SECTION("width 0")
    {
        const auto file = bytesOf({0x4D, 0x52, 0x50, 0x48, 0, 0, 0, 0, 3, 0, 0, 0});
        CHECK_FALSE(decodeMap(file).has_value());
    }
    SECTION("height 0")
    {
        const auto file = bytesOf({0x4D, 0x52, 0x50, 0x48, 3, 0, 0, 0, 0, 0, 0, 0});
        CHECK_FALSE(decodeMap(file).has_value());
    }
}

TEST_CASE("decodeMap rejects a header claiming the largest possible map", "[util][decode][edge]")
{
    // 0xFFFFFFFF * 0xFFFFFFFF needs 64 bits; the 9 cells present can't match it.
    auto file = smallRoomFile();
    for (std::size_t i = 4; i < 12; ++i)
    {
        file[i] = std::byte{0xFF};
    }

    CHECK_FALSE(decodeMap(file).has_value());
}

// ---------------------------------------------------------------- Map::cell

TEST_CASE("Map::cell indexes row by row", "[util][cell]")
{
    const Map map = wideMap();

    const auto right = map.cell(1, 0); // one step along x
    const auto down = map.cell(0, 1);  // one step along y = one full row

    REQUIRE(right.has_value());
    REQUIRE(down.has_value());
    CHECK(int{*right} == 1); // int so Catch2 prints a number, not a char
    CHECK(int{*down} == 252);
}

TEST_CASE("Map::cell returns the last cell at (width-1, height-1)", "[util][cell][edge]")
{
    const auto last = wideMap().cell(3, 1);

    REQUIRE(last.has_value());
    CHECK(int{*last} == 255);
}

TEST_CASE("Map::cell returns nullopt outside the map", "[util][cell][edge]")
{
    const Map map = wideMap();
    constexpr auto kMax = std::numeric_limits<std::uint32_t>::max();

    CHECK_FALSE(map.cell(4, 0).has_value());       // x == width
    CHECK_FALSE(map.cell(0, 2).has_value());       // y == height
    CHECK_FALSE(map.cell(kMax, kMax).has_value());
}

TEST_CASE("a default-constructed Map has no cells to read", "[util][cell][edge]")
{
    // A Map with no cells must not report a readable cell. If width and height
    // default to anything but 0, cell(0, 0) indexes an empty vector.
    const Map map;

    CHECK(map.cells.empty());
    CHECK_FALSE(map.cell(0, 0).has_value());
}
