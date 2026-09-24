#include <io.hpp>
#include <stdexcept>
#include <filesystem>
#include <fstream>
#include <ios>
#include <iosfwd>

namespace morph::io
{
std::vector<std::byte> readFile(const std::filesystem::path& path)
{
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file)
    {
        throw std::runtime_error("Failed to open file.");
    }

    const auto end = file.tellg();
    if (end == std::ifstream::pos_type(-1))
    {
        throw std::runtime_error("Failed to determine file size");
    }

    std::vector<std::byte> bytes(static_cast<std::size_t>(end));

    file.seekg(0, std::ios::beg);
    if (!file)
    {
        throw std::runtime_error("Failed to seek to beginning");
    }

    if ( !bytes.empty() && !file.read(reinterpret_cast<char*>(bytes.data()), static_cast<std::streamsize>(bytes.size())) )
    {
        throw std::runtime_error("Failed to read entire file");
    }

    return bytes;
}

void writeFile(const std::filesystem::path& path, std::span<std::byte> bytes)
{
    std::ofstream file(path, std::ios::binary);
    if (!file)
    {
        throw std::runtime_error("Failed to open file.");
    }

    file.write(reinterpret_cast<char*>(bytes.data()), static_cast<std::streamsize>(bytes.size()));
}
}
