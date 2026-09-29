#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#include <io.hpp>

#include <cstddef>
#include <filesystem>
#include <span>
#include <stdexcept>
#include <string>
#include <string_view>
#include <system_error>
#include <vector>

using namespace std::literals;
using morph::io::readBinaryFile;
using morph::io::writeBinaryFile;

namespace fs = std::filesystem;

namespace
{
std::vector<std::byte> toBytes(std::string_view text)
{
    const auto view = std::as_bytes(std::span(text));
    return {view.begin(), view.end()};
}

// A path in the system temp directory that is removed when the test ends.
// Each test uses its own name so ctest can run them in parallel.
class TempFile
{
public:
    explicit TempFile(std::string_view name)
        : m_path(fs::temp_directory_path() / ("morph_io_tests_" + std::string(name)))
    {
        std::error_code ignored;
        fs::remove(m_path, ignored); // leftovers from a crashed run
    }

    ~TempFile()
    {
        std::error_code ignored;
        fs::remove(m_path, ignored);
    }

    TempFile(const TempFile&) = delete;
    TempFile& operator=(const TempFile&) = delete;

    const fs::path& path() const { return m_path; }

private:
    fs::path m_path;
};
} // namespace

TEST_CASE("writeBinaryFile then readBinaryFile returns the same bytes", "[io]")
{
    const TempFile file("hello.bin");
    const auto bytes = toBytes("hello"sv);

    writeBinaryFile(file.path(), bytes);

    CHECK(readBinaryFile(file.path()) == bytes);
}

TEST_CASE("every byte value survives a write and read", "[io]")
{
    // Text mode would translate bytes such as '\n' (0x0A) or 0x1A on some
    // platforms; binary mode must not change any of the 256 values.
    const TempFile file("all_bytes.bin");
    std::vector<std::byte> bytes;
    for (int value = 0; value < 256; ++value)
    {
        bytes.push_back(static_cast<std::byte>(value));
    }

    writeBinaryFile(file.path(), bytes);

    CHECK(readBinaryFile(file.path()) == bytes);
}

TEST_CASE("an empty file round-trips", "[io][edge]")
{
    const TempFile file("empty.bin");

    writeBinaryFile(file.path(), std::span<const std::byte>{});

    CHECK(fs::file_size(file.path()) == 0);
    CHECK(readBinaryFile(file.path()).empty());
}

TEST_CASE("writeBinaryFile replaces the previous contents", "[io]")
{
    // Saving a smaller map over a larger one must not leave the old tail behind.
    const TempFile file("overwrite.bin");

    writeBinaryFile(file.path(), toBytes("abcdef"sv));
    writeBinaryFile(file.path(), toBytes("xy"sv));

    CHECK(readBinaryFile(file.path()) == toBytes("xy"sv));
}

TEST_CASE("readBinaryFile throws for a missing file and names it", "[io][error]")
{
    const TempFile missing("does_not_exist.bin");

    CHECK_THROWS_AS(readBinaryFile(missing.path()), std::runtime_error);
    CHECK_THROWS_WITH(readBinaryFile(missing.path()),
                      Catch::Matchers::ContainsSubstring(missing.path().string()));
}

TEST_CASE("writeBinaryFile throws when the directory doesn't exist", "[io][error]")
{
    const auto path = fs::temp_directory_path() / "morph_io_tests_no_such_dir" / "out.bin";

    CHECK_THROWS_AS(writeBinaryFile(path, toBytes("x"sv)), std::runtime_error);
}

TEST_CASE("writeBinaryFile throws when the disk is full", "[io][error]")
{
    // /dev/full accepts the open but fails every write with "no space left".
    // It exists on Linux; elsewhere this test is skipped.
    const fs::path deviceFull = "/dev/full";
    if (!fs::exists(deviceFull))
    {
        SKIP("/dev/full is not available on this platform");
    }

    const std::vector<std::byte> bytes(64 * 1024, std::byte{0x41});

    CHECK_THROWS_AS(writeBinaryFile(deviceFull, bytes), std::runtime_error);
}
