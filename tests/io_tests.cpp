#include <catch2/catch_test_macros.hpp>
#include <span>
#include <string_view>
#include <io.hpp>
#include <vector>


using namespace std::literals;

std::vector<std::byte> toBytes(std::string_view text)
{
    const auto view = std::as_bytes(std::span(text));
    return {view.begin(), view.end()};
}

TEST_CASE("readFile and writeFile with a few characters", "[io]")
{
    auto bytes = toBytes("hello"sv);
    morph::io::writeFile("out.bin", bytes);

    CHECK(morph::io::readFile("out.bin") == bytes);
}
