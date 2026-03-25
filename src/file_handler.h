#pragma once
#include <vector>
#include <string>
#include <cstdint>

/**
 * File I/O helpers.
 * Reads a file into a byte vector and writes a byte vector to a file.
 */
class FileHandler {
public:
    static std::vector<uint8_t> readFile(const std::string& path);
    static void writeFile(const std::string& path, const std::vector<uint8_t>& data);
};
