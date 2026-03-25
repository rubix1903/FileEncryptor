#pragma once
#include <vector>
#include <string>
#include <cstdint>

/**
 * XOR Cipher
 * Symmetric encryption: XOR each byte of plaintext with a repeating key.
 * The same function is used for both encryption and decryption.
 */
class XorCipher {
public:
    /**
     * Encrypt or decrypt data using XOR with a repeating key.
     * @param data   Input bytes (plaintext or ciphertext)
     * @param key    Key string (repeated as needed)
     * @return       Transformed bytes
     */
    static std::vector<uint8_t> process(const std::vector<uint8_t>& data,
                                        const std::string& key);
};
