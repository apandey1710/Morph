#pragma once

#include <cstddef>
#include <filesystem>
#include <span>
#include <vector>

namespace morph::io
{
    std::vector<std::byte> readBinaryFile(const std::filesystem::path& path);
    void writeBinaryFile(const std::filesystem::path& path, std::span<const std::byte> bytes);
}
