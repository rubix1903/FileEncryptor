#pragma once
#include <vector>
#include <array>
#include <cstdint>
#include <string>

/**
 * AES-128 in CBC mode (Cipher Block Chaining)
 * Block size : 16 bytes (128 bits)
 * Key size   : 16 bytes (128 bits) — derived from a passphrase via SHA-256
 * Padding    : PKCS#7
 * IV         : 16 random bytes prepended to the ciphertext
 */
class AesCipher {
public:
    static constexpr size_t BLOCK_SIZE = 16;
    static constexpr size_t KEY_SIZE   = 16;  // AES-128

    /**
     * Encrypting plaintext bytes with AES-128-CBC.
     * A random IV is generated and prepended to the returned ciphertext.
     */
    static std::vector<uint8_t> encrypt(const std::vector<uint8_t>& plaintext,
                                        const std::string& passphrase);

    /**
     * Decrypting ciphertext (IV prepended) with AES-128-CBC.
     */
    static std::vector<uint8_t> decrypt(const std::vector<uint8_t>& ciphertext,
                                        const std::string& passphrase);

private:
    using Block = std::array<uint8_t, BLOCK_SIZE>;
    using RoundKeys = std::array<std::array<uint8_t, BLOCK_SIZE>, 11>;

    // Key derivation
    static std::array<uint8_t, KEY_SIZE> deriveKey(const std::string& passphrase);

    // AES key schedule
    static RoundKeys expandKey(const std::array<uint8_t, KEY_SIZE>& key);

    // Core AES block operations
    static Block encryptBlock(Block block, const RoundKeys& rk);
    static Block decryptBlock(Block block, const RoundKeys& rk);

    // AES sub-operations
    static void subBytes(Block& b);
    static void invSubBytes(Block& b);
    static void shiftRows(Block& b);
    static void invShiftRows(Block& b);
    static void mixColumns(Block& b);
    static void invMixColumns(Block& b);
    static void addRoundKey(Block& b, const std::array<uint8_t, 16>& rk);

    // GF(2^8) multiplication
    static uint8_t gmul(uint8_t a, uint8_t b);

    // Random IV generation
    static Block randomIV();

    // PKCS#7 padding
    static std::vector<uint8_t> pkcs7Pad(const std::vector<uint8_t>& data);
    static std::vector<uint8_t> pkcs7Unpad(const std::vector<uint8_t>& data);
};
