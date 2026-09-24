#include <catch2/catch_test_macros.hpp>
#include <io.hpp>
#include "temp_directory.hpp"
#include <array>
#include <fstream>
#include <iterator>
#include <stdexcept>
#include <vector>

namespace
{
std::vector<std::byte> payload()
{
    return {std::byte{0x00}, std::byte{0x0A}, std::byte{0x0D},
            std::byte{0x7F}, std::byte{0x80}, std::byte{0xFF}};
}

// Independent setup: do not use writeFile to test readFile.
void seed(const std::filesystem::path& path, std::span<const std::byte> bytes)
{
    std::ofstream file(path, std::ios::binary);
    REQUIRE(file.is_open());
    if (!bytes.empty())
    {
        file.write(reinterpret_cast<const char*>(bytes.data()),
                   static_cast<std::streamsize>(bytes.size()));
    }
    file.close();
    REQUIRE(file.good());
}

// Independent observation: do not use readFile to test writeFile.
std::vector<std::byte> inspect(const std::filesystem::path& path)
{
    std::ifstream file(path, std::ios::binary);
    REQUIRE(file.is_open());
    std::vector<std::byte> bytes;
    char value;
    while (file.get(value))
    {
        bytes.push_back(static_cast<std::byte>(static_cast<unsigned char>(value)));
    }
    REQUIRE(file.eof());
    REQUIRE_FALSE(file.bad());
    return bytes;
}
}

TEST_CASE("readFile preserves binary bytes", "[io][read]")
{
    TempDirectory directory;
    const auto path = directory.file("binary.bin");
    const auto expected = payload();
    seed(path, expected);

    const auto actual = morph::io::readFile(path);

    REQUIRE(actual == expected);
}

TEST_CASE("readFile accepts an empty file", "[io][read]")
{
    TempDirectory directory;
    const auto path = directory.file("empty.bin");
    seed(path, {});
    REQUIRE(morph::io::readFile(path).empty());
}

TEST_CASE("readFile rejects a missing file", "[io][read]")
{
    TempDirectory directory;
    const auto path = directory.file("missing.bin");
    REQUIRE_FALSE(std::filesystem::exists(path));
    REQUIRE_THROWS_AS(morph::io::readFile(path), std::runtime_error);
}

TEST_CASE("writeFile creates an exact binary file", "[io][write]")
{
    TempDirectory directory;
    const auto path = directory.file("new.bin");
    auto expected = payload();
    morph::io::writeFile(path, expected);
    REQUIRE(inspect(path) == expected);
    CHECK(std::filesystem::file_size(path) == expected.size());
}

TEST_CASE("writeFile truncates a longer file", "[io][write]")
{
    TempDirectory directory;
    const auto path = directory.file("overwrite.bin");
    seed(path, payload());
    std::vector<std::byte> shorter{std::byte{0x42}};
    morph::io::writeFile(path, shorter);
    REQUIRE(inspect(path) == shorter);
    CHECK(std::filesystem::file_size(path) == 1);
}

TEST_CASE("writeFile creates and truncates empty files", "[io][write]")
{
    TempDirectory directory;
    const auto path = directory.file("empty.bin");
    SECTION("new file") {}
    SECTION("existing file") { seed(path, payload()); }
    std::vector<std::byte> empty;
    morph::io::writeFile(path, empty);
    REQUIRE(std::filesystem::exists(path));
    CHECK(std::filesystem::file_size(path) == 0);
    CHECK(inspect(path).empty());
}

TEST_CASE("writeFile rejects a missing parent", "[io][write]")
{
    TempDirectory directory;
    const auto path = directory.file("absent") / "output.bin";
    auto bytes = payload();
    REQUIRE_THROWS_AS(morph::io::writeFile(path, bytes), std::runtime_error);
}

TEST_CASE("file IO round trip preserves bytes", "[io][roundtrip]")
{
    TempDirectory directory;
    const auto path = directory.file("roundtrip.bin");
    auto expected = payload();
    morph::io::writeFile(path, expected);
    REQUIRE(morph::io::readFile(path) == expected);
}
