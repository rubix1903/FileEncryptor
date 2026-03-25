#include "file_handler.h"
#include <fstream>
#include <stdexcept>
#include <iterator>

std::vector<uint8_t> FileHandler::readFile(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open())
        throw std::runtime_error("Cannot open input file: " + path);

    return std::vector<uint8_t>(
        std::istreambuf_iterator<char>(file),
        std::istreambuf_iterator<char>()
    );
}

void FileHandler::writeFile(const std::string& path, const std::vector<uint8_t>& data) {
    std::ofstream file(path, std::ios::binary | std::ios::trunc);
    if (!file.is_open())
        throw std::runtime_error("Cannot open output file: " + path);

    file.write(reinterpret_cast<const char*>(data.data()),
               static_cast<std::streamsize>(data.size()));

    if (!file.good())
        throw std::runtime_error("Write error on file: " + path);
}
