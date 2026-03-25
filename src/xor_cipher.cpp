#include "xor_cipher.h"
#include <stdexcept>

std::vector<uint8_t> XorCipher::process(const std::vector<uint8_t>& data,
                                         const std::string& key)
{
    if (key.empty()) {
        throw std::invalid_argument("XOR key must not be empty.");
    }

    std::vector<uint8_t> result(data.size());
    const size_t keyLen = key.size();

    for (size_t i = 0; i < data.size(); ++i) {
        // Bitwise XOR of each data byte with the corresponding key byte (cycled)
        result[i] = data[i] ^ static_cast<uint8_t>(key[i % keyLen]);
    }

    return result;
}
