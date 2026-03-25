# fencrypt — File Encryption Utility

A command-line file encryption tool with AES-128-CBC and XOR ciphers.
Displays a full ASCII art banner on every run.

```
  ███████╗███╗   ██╗ ██████╗██████╗ ██╗   ██╗██████╗ ████████╗
  ██╔════╝████╗  ██║██╔════╝██╔══██╗╚██╗ ██╔╝██╔══██╗╚══██╔══╝
  █████╗  ██╔██╗ ██║██║     ██████╔╝ ╚████╔╝ ██████╔╝   ██║
  ██╔══╝  ██║╚██╗██║██║     ██╔══██╗  ╚██╔╝  ██╔═══╝    ██║
  ███████╗██║ ╚████║╚██████╗██║  ██║   ██║   ██║        ██║
  ╚══════╝╚═╝  ╚═══╝ ╚═════╝╚═╝  ╚═╝   ╚═╝   ╚═╝        ╚═╝
  ──────────────────────────────────────────────────────────────
    File Encryption Utility  |  AES-128-CBC & XOR  |  v1.0.0
  ──────────────────────────────────────────────────────────────
```

---
## Project Structure

```
file_encryptor/
├── CMakeLists.txt
├── README.md
├── LICENSE
├── install-linux.sh
├── install-mac.sh
├── install-windows.bat
└── src/
    ├── main.cpp
    ├── aes_cipher.h/.cpp
    ├── xor_cipher.h/.cpp
    ├── file_handler.h/.cpp
    └── utils.h/.cpp
```

## Install

### Linux
```bash
git clone <repo-url>
cd file_encryptor
chmod +x install-linux.sh
./install-linux.sh
```

### macOS
```bash
git clone <repo-url>
cd file_encryptor
chmod +x install-mac.sh
./install-mac.sh
```

### Windows
```cmd
# Open cmd or PowerShell as Administrator
install-windows.bat
```

After install, open a **new terminal** and run:
```bash
fencrypt --help
```

---

## Usage

```
fencrypt <mode> <algorithm> <input_file> <output_file>

Modes:       encrypt | decrypt
Algorithms:  aes | xor

fencrypt encrypt aes  secret.txt  secret.enc
fencrypt decrypt aes  secret.enc  secret.txt
fencrypt encrypt xor  photo.jpg   photo.xor
fencrypt --version
fencrypt --help
```

---

## Algorithms

### AES-128-CBC
- **Block size**: 128 bits (16 bytes)
- **Key**: 128-bit key derived from passphrase using hash stretching
- **Mode**: CBC — each block is XORed with the previous ciphertext block before encryption
- **IV**: 16 random bytes, prepended to the output (re-read automatically on decrypt)
- **Padding**: PKCS#7 — ensures the plaintext is a multiple of 16 bytes

### XOR Cipher
- Each byte of the input is XORed with the corresponding byte of a repeating key
- Symmetric: the same operation encrypts and decrypts
- Simple, fast, suitable for learning/obfuscation

---

## Notes

- **Passphrase strength** matters for AES security — use a long, random passphrase.
- XOR with a key shorter than the plaintext is not cryptographically secure. Use AES for sensitive data.
- For production use, replace the built-in key derivation with a proper KDF such as PBKDF2 or Argon2.


