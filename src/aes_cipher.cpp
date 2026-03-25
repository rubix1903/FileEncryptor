#include "aes_cipher.h"
#include <stdexcept>
#include <cstring>
#include <random>
#include <functional>

// ─── AES S-Box and inverse S-Box ─────────────────────────────────────────────

static const uint8_t SBOX[256] = {
    0x63,0x7c,0x77,0x7b,0xf2,0x6b,0x6f,0xc5,0x30,0x01,0x67,0x2b,0xfe,0xd7,0xab,0x76,
    0xca,0x82,0xc9,0x7d,0xfa,0x59,0x47,0xf0,0xad,0xd4,0xa2,0xaf,0x9c,0xa4,0x72,0xc0,
    0xb7,0xfd,0x93,0x26,0x36,0x3f,0xf7,0xcc,0x34,0xa5,0xe5,0xf1,0x71,0xd8,0x31,0x15,
    0x04,0xc7,0x23,0xc3,0x18,0x96,0x05,0x9a,0x07,0x12,0x80,0xe2,0xeb,0x27,0xb2,0x75,
    0x09,0x83,0x2c,0x1a,0x1b,0x6e,0x5a,0xa0,0x52,0x3b,0xd6,0xb3,0x29,0xe3,0x2f,0x84,
    0x53,0xd1,0x00,0xed,0x20,0xfc,0xb1,0x5b,0x6a,0xcb,0xbe,0x39,0x4a,0x4c,0x58,0xcf,
    0xd0,0xef,0xaa,0xfb,0x43,0x4d,0x33,0x85,0x45,0xf9,0x02,0x7f,0x50,0x3c,0x9f,0xa8,
    0x51,0xa3,0x40,0x8f,0x92,0x9d,0x38,0xf5,0xbc,0xb6,0xda,0x21,0x10,0xff,0xf3,0xd2,
    0xcd,0x0c,0x13,0xec,0x5f,0x97,0x44,0x17,0xc4,0xa7,0x7e,0x3d,0x64,0x5d,0x19,0x73,
    0x60,0x81,0x4f,0xdc,0x22,0x2a,0x90,0x88,0x46,0xee,0xb8,0x14,0xde,0x5e,0x0b,0xdb,
    0xe0,0x32,0x3a,0x0a,0x49,0x06,0x24,0x5c,0xc2,0xd3,0xac,0x62,0x91,0x95,0xe4,0x79,
    0xe7,0xc8,0x37,0x6d,0x8d,0xd5,0x4e,0xa9,0x6c,0x56,0xf4,0xea,0x65,0x7a,0xae,0x08,
    0xba,0x78,0x25,0x2e,0x1c,0xa6,0xb4,0xc6,0xe8,0xdd,0x74,0x1f,0x4b,0xbd,0x8b,0x8a,
    0x70,0x3e,0xb5,0x66,0x48,0x03,0xf6,0x0e,0x61,0x35,0x57,0xb9,0x86,0xc1,0x1d,0x9e,
    0xe1,0xf8,0x98,0x11,0x69,0xd9,0x8e,0x94,0x9b,0x1e,0x87,0xe9,0xce,0x55,0x28,0xdf,
    0x8c,0xa1,0x89,0x0d,0xbf,0xe6,0x42,0x68,0x41,0x99,0x2d,0x0f,0xb0,0x54,0xbb,0x16,
};

static const uint8_t INV_SBOX[256] = {
    0x52,0x09,0x6a,0xd5,0x30,0x36,0xa5,0x38,0xbf,0x40,0xa3,0x9e,0x81,0xf3,0xd7,0xfb,
    0x7c,0xe3,0x39,0x82,0x9b,0x2f,0xff,0x87,0x34,0x8e,0x43,0x44,0xc4,0xde,0xe9,0xcb,
    0x54,0x7b,0x94,0x32,0xa6,0xc2,0x23,0x3d,0xee,0x4c,0x95,0x0b,0x42,0xfa,0xc3,0x4e,
    0x08,0x2e,0xa1,0x66,0x28,0xd9,0x24,0xb2,0x76,0x5b,0xa2,0x49,0x6d,0x8b,0xd1,0x25,
    0x72,0xf8,0xf6,0x64,0x86,0x68,0x98,0x16,0xd4,0xa4,0x5c,0xcc,0x5d,0x65,0xb6,0x92,
    0x6c,0x70,0x48,0x50,0xfd,0xed,0xb9,0xda,0x5e,0x15,0x46,0x57,0xa7,0x8d,0x9d,0x84,
    0x90,0xd8,0xab,0x00,0x8c,0xbc,0xd3,0x0a,0xf7,0xe4,0x58,0x05,0xb8,0xb3,0x45,0x06,
    0xd0,0x2c,0x1e,0x8f,0xca,0x3f,0x0f,0x02,0xc1,0xaf,0xbd,0x03,0x01,0x13,0x8a,0x6b,
    0x3a,0x91,0x11,0x41,0x4f,0x67,0xdc,0xea,0x97,0xf2,0xcf,0xce,0xf0,0xb4,0xe6,0x73,
    0x96,0xac,0x74,0x22,0xe7,0xad,0x35,0x85,0xe2,0xf9,0x37,0xe8,0x1c,0x75,0xdf,0x6e,
    0x47,0xf1,0x1a,0x71,0x1d,0x29,0xc5,0x89,0x6f,0xb7,0x62,0x0e,0xaa,0x18,0xbe,0x1b,
    0xfc,0x56,0x3e,0x4b,0xc6,0xd2,0x79,0x20,0x9a,0xdb,0xc0,0xfe,0x78,0xcd,0x5a,0xf4,
    0x1f,0xdd,0xa8,0x33,0x88,0x07,0xc7,0x31,0xb1,0x12,0x10,0x59,0x27,0x80,0xec,0x5f,
    0x60,0x51,0x7f,0xa9,0x19,0xb5,0x4a,0x0d,0x2d,0xe5,0x7a,0x9f,0x93,0xc9,0x9c,0xef,
    0xa0,0xe0,0x3b,0x4d,0xae,0x2a,0xf5,0xb0,0xc8,0xeb,0xbb,0x3c,0x83,0x53,0x99,0x61,
    0x17,0x2b,0x04,0x7e,0xba,0x77,0xd6,0x26,0xe1,0x69,0x14,0x63,0x55,0x21,0x0c,0x7d,
};

// Rcon for key schedule
static const uint8_t RCON[11] = {
    0x00,0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,0x1b,0x36
};

// ─── GF(2^8) multiply ────────────────────────────────────────────────────────

uint8_t AesCipher::gmul(uint8_t a, uint8_t b) {
    uint8_t result = 0;
    for (int i = 0; i < 8; ++i) {
        if (b & 1) result ^= a;
        bool hi = (a & 0x80) != 0;
        a <<= 1;
        if (hi) a ^= 0x1b;  // x^8 + x^4 + x^3 + x + 1
        b >>= 1;
    }
    return result;
}

// ─── Key derivation (simple SHA-256-like using std::hash for portability) ─────
// uses a basic PBKDF: hash the passphrase repeatedly to fill 16 bytes.

static void simpleHash(const std::string& input, uint8_t* out, size_t outLen) {
    // Simple deterministic key stretching: XOR-fold std::hash over the passphrase
    // with salt bytes derived from its length and position.
    std::vector<uint8_t> buf(outLen, 0);
    for (size_t i = 0; i < outLen; ++i) {
        size_t h = std::hash<std::string>{}(input + std::to_string(i));
        buf[i] = static_cast<uint8_t>(h ^ (h >> 8) ^ (h >> 16) ^ (h >> 24));
    }
    // Extra mixing pass
    for (size_t round = 0; round < 1000; ++round) {
        for (size_t i = 0; i < outLen; ++i) {
            size_t h = std::hash<std::string>{}(
                input + std::to_string(round) + std::to_string(buf[i])
            );
            buf[i] ^= static_cast<uint8_t>(h ^ (h >> 16));
        }
    }
    std::memcpy(out, buf.data(), outLen);
}

std::array<uint8_t, AesCipher::KEY_SIZE> AesCipher::deriveKey(const std::string& passphrase) {
    std::array<uint8_t, KEY_SIZE> key{};
    simpleHash(passphrase, key.data(), KEY_SIZE);
    return key;
}

// ─── Key Expansion ────────────────────────────────────────────────────────────

AesCipher::RoundKeys AesCipher::expandKey(const std::array<uint8_t, KEY_SIZE>& key) {
    RoundKeys rk{};
    // Copy original key into round key 0
    std::memcpy(rk[0].data(), key.data(), KEY_SIZE);

    for (int i = 1; i <= 10; ++i) {
        auto& prev = rk[i - 1];
        auto& curr = rk[i];

        // RotWord + SubWord + Rcon on last word of previous round key
        uint8_t temp[4] = {
            static_cast<uint8_t>(SBOX[prev[13]] ^ RCON[i]),
            SBOX[prev[14]],
            SBOX[prev[15]],
            SBOX[prev[12]],
        };

        for (int j = 0; j < 4; ++j) curr[j]      = prev[j]      ^ temp[j];
        for (int j = 0; j < 4; ++j) curr[4 + j]  = prev[4 + j]  ^ curr[j];
        for (int j = 0; j < 4; ++j) curr[8 + j]  = prev[8 + j]  ^ curr[4 + j];
        for (int j = 0; j < 4; ++j) curr[12 + j] = prev[12 + j] ^ curr[8 + j];
    }
    return rk;
}

// ─── AES Round Operations ─────────────────────────────────────────────────────

void AesCipher::subBytes(Block& b) {
    for (auto& byte : b) byte = SBOX[byte];
}

void AesCipher::invSubBytes(Block& b) {
    for (auto& byte : b) byte = INV_SBOX[byte];
}

void AesCipher::shiftRows(Block& b) {
    // Row 1: shift left 1
    uint8_t t = b[1]; b[1]=b[5]; b[5]=b[9]; b[9]=b[13]; b[13]=t;
    // Row 2: shift left 2
    std::swap(b[2], b[10]); std::swap(b[6], b[14]);
    // Row 3: shift left 3 (= shift right 1)
    t = b[15]; b[15]=b[11]; b[11]=b[7]; b[7]=b[3]; b[3]=t;
}

void AesCipher::invShiftRows(Block& b) {
    // Row 1: shift right 1
    uint8_t t = b[13]; b[13]=b[9]; b[9]=b[5]; b[5]=b[1]; b[1]=t;
    // Row 2: shift right 2
    std::swap(b[2], b[10]); std::swap(b[6], b[14]);
    // Row 3: shift right 3 (= shift left 1)
    t = b[3]; b[3]=b[7]; b[7]=b[11]; b[11]=b[15]; b[15]=t;
}

void AesCipher::mixColumns(Block& b) {
    for (int c = 0; c < 4; ++c) {
        uint8_t s0 = b[c*4], s1 = b[c*4+1], s2 = b[c*4+2], s3 = b[c*4+3];
        b[c*4]   = gmul(0x02,s0)^gmul(0x03,s1)^s2^s3;
        b[c*4+1] = s0^gmul(0x02,s1)^gmul(0x03,s2)^s3;
        b[c*4+2] = s0^s1^gmul(0x02,s2)^gmul(0x03,s3);
        b[c*4+3] = gmul(0x03,s0)^s1^s2^gmul(0x02,s3);
    }
}

void AesCipher::invMixColumns(Block& b) {
    for (int c = 0; c < 4; ++c) {
        uint8_t s0 = b[c*4], s1 = b[c*4+1], s2 = b[c*4+2], s3 = b[c*4+3];
        b[c*4]   = gmul(0x0e,s0)^gmul(0x0b,s1)^gmul(0x0d,s2)^gmul(0x09,s3);
        b[c*4+1] = gmul(0x09,s0)^gmul(0x0e,s1)^gmul(0x0b,s2)^gmul(0x0d,s3);
        b[c*4+2] = gmul(0x0d,s0)^gmul(0x09,s1)^gmul(0x0e,s2)^gmul(0x0b,s3);
        b[c*4+3] = gmul(0x0b,s0)^gmul(0x0d,s1)^gmul(0x09,s2)^gmul(0x0e,s3);
    }
}

void AesCipher::addRoundKey(Block& b, const std::array<uint8_t, 16>& rk) {
    for (int i = 0; i < 16; ++i) b[i] ^= rk[i];
}

// ─── AES Block Encrypt / Decrypt ─────────────────────────────────────────────

AesCipher::Block AesCipher::encryptBlock(Block block, const RoundKeys& rk) {
    addRoundKey(block, rk[0]);
    for (int round = 1; round <= 9; ++round) {
        subBytes(block);
        shiftRows(block);
        mixColumns(block);
        addRoundKey(block, rk[round]);
    }
    subBytes(block);
    shiftRows(block);
    addRoundKey(block, rk[10]);
    return block;
}

AesCipher::Block AesCipher::decryptBlock(Block block, const RoundKeys& rk) {
    addRoundKey(block, rk[10]);
    for (int round = 9; round >= 1; --round) {
        invShiftRows(block);
        invSubBytes(block);
        addRoundKey(block, rk[round]);
        invMixColumns(block);
    }
    invShiftRows(block);
    invSubBytes(block);
    addRoundKey(block, rk[0]);
    return block;
}

// ─── Random IV ───────────────────────────────────────────────────────────────

AesCipher::Block AesCipher::randomIV() {
    Block iv{};
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint16_t> dist(0, 255);
    for (auto& b : iv) b = static_cast<uint8_t>(dist(gen));
    return iv;
}

// ─── PKCS#7 Padding ──────────────────────────────────────────────────────────

std::vector<uint8_t> AesCipher::pkcs7Pad(const std::vector<uint8_t>& data) {
    size_t padLen = BLOCK_SIZE - (data.size() % BLOCK_SIZE);
    std::vector<uint8_t> padded(data);
    padded.insert(padded.end(), padLen, static_cast<uint8_t>(padLen));
    return padded;
}

std::vector<uint8_t> AesCipher::pkcs7Unpad(const std::vector<uint8_t>& data) {
    if (data.empty() || data.size() % BLOCK_SIZE != 0)
        throw std::runtime_error("Invalid ciphertext length (bad padding).");
    uint8_t padLen = data.back();
    if (padLen == 0 || padLen > BLOCK_SIZE)
        throw std::runtime_error("Invalid PKCS#7 padding value.");
    for (size_t i = data.size() - padLen; i < data.size(); ++i)
        if (data[i] != padLen)
            throw std::runtime_error("Corrupted PKCS#7 padding.");
    return std::vector<uint8_t>(data.begin(), data.end() - padLen);
}

// ─── Public API ──────────────────────────────────────────────────────────────

std::vector<uint8_t> AesCipher::encrypt(const std::vector<uint8_t>& plaintext,
                                         const std::string& passphrase)
{
    auto key = deriveKey(passphrase);
    auto rk  = expandKey(key);
    Block iv  = randomIV();

    auto padded = pkcs7Pad(plaintext);
    std::vector<uint8_t> ciphertext(iv.begin(), iv.end()); // prepend IV
    ciphertext.reserve(BLOCK_SIZE + padded.size());

    Block prev = iv;  // CBC: XOR each plaintext block with previous ciphertext block
    for (size_t i = 0; i < padded.size(); i += BLOCK_SIZE) {
        Block block{};
        std::memcpy(block.data(), padded.data() + i, BLOCK_SIZE);

        // CBC XOR
        for (int j = 0; j < 16; ++j) block[j] ^= prev[j];

        prev = encryptBlock(block, rk);
        ciphertext.insert(ciphertext.end(), prev.begin(), prev.end());
    }
    return ciphertext;
}

std::vector<uint8_t> AesCipher::decrypt(const std::vector<uint8_t>& ciphertext,
                                         const std::string& passphrase)
{
    if (ciphertext.size() < BLOCK_SIZE * 2)
        throw std::runtime_error("Ciphertext too short to contain IV + data.");

    auto key = deriveKey(passphrase);
    auto rk  = expandKey(key);

    // Extract IV
    Block iv{};
    std::memcpy(iv.data(), ciphertext.data(), BLOCK_SIZE);

    std::vector<uint8_t> decrypted;
    decrypted.reserve(ciphertext.size() - BLOCK_SIZE);

    Block prev = iv;
    for (size_t i = BLOCK_SIZE; i < ciphertext.size(); i += BLOCK_SIZE) {
        Block block{};
        std::memcpy(block.data(), ciphertext.data() + i, BLOCK_SIZE);

        Block plain = decryptBlock(block, rk);
        // CBC XOR
        for (int j = 0; j < 16; ++j) plain[j] ^= prev[j];

        decrypted.insert(decrypted.end(), plain.begin(), plain.end());
        prev = block;
    }
    return pkcs7Unpad(decrypted);
}
