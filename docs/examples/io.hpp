#pragma once
#include <cstddef>
#include <filesystem>
#include <span>
#include <vector>

namespace morph::io
{
// Future file I/O declarations belong here.
    std::vector<std::byte> readFile(const std::filesystem::path& path);
    void writeFile(const std::filesystem::path& path, std::span<std::byte> bytes);
}
