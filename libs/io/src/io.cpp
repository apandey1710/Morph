#include <io.hpp>
#include <filesystem>
#include <fstream>
#include <ios>
#include <stdexcept>
#include <string>

namespace morph::io
{
std::vector<std::byte> readBinaryFile(const std::filesystem::path& path)
{
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file)
    {
        throw std::runtime_error("Failed to open file " + path.string());
    }

    const auto end = file.tellg();
    if (end == std::ifstream::pos_type(-1))
    {
        throw std::runtime_error("Failed to determine file size for file " + path.string());
    }

    std::vector<std::byte> bytes(static_cast<std::size_t>(end));

    file.seekg(0, std::ios::beg);
    if (!file)
    {
        throw std::runtime_error("Failed to seek to beginning in file " + path.string());
    }

    if ( !bytes.empty() && !file.read(reinterpret_cast<char*>(bytes.data()), static_cast<std::streamsize>(bytes.size())) )
    {
        throw std::runtime_error("Failed to read entire file " + path.string());
    }

    file.close();
    return bytes;
}

void writeBinaryFile(const std::filesystem::path& path, std::span<const std::byte> bytes)
{
    std::ofstream file(path, std::ios::binary);
    if (!file)
    {
        throw std::runtime_error("Failed to open file " + path.string());
    }

    file.write(reinterpret_cast<const char*>(bytes.data()), static_cast<std::streamsize>(bytes.size()));
    file.close();

    if (!file)
    {
        throw std::runtime_error("Failed to write " + path.string());
    }
}

} // namespace morph::io
