#include <iostream>
#include <string>
#include <algorithm>

#include "xor_cipher.h"
#include "aes_cipher.h"
#include "file_handler.h"
#include "utils.h"

int main(int argc, char* argv[]) {
    const std::string prog = argv[0];


    Utils::printBanner();

    if (argc < 2) {
        Utils::printHelp(prog);
        return 0;
    }

    std::string firstArg = argv[1];
    std::transform(firstArg.begin(), firstArg.end(), firstArg.begin(), ::tolower);

    if (firstArg == "--help" || firstArg == "-h") {
        Utils::printHelp(prog);
        return 0;
    }

    if (firstArg == "--version" || firstArg == "-v") {
        std::cout << "\033[1;37m" FENC_NAME "\033[0m  version \033[1;33m" FENC_VERSION "\033[0m\n"
                  << "Algorithms : AES-128-CBC, XOR\n"
                  << "Build      : C++17 / CMake\n\n";
        return 0;
    }

    if (argc != 5) {
        std::cerr << "\033[1;31m[ERROR]\033[0m Expected 4 arguments: <mode> <algorithm> <input> <output>\n"
                  << "Run '\033[1;33m" << prog << " --help\033[0m' for usage.\n\n";
        return 1;
    }

    std::string mode    = argv[1];
    std::string algo    = argv[2];
    std::string inPath  = argv[3];
    std::string outPath = argv[4];

    std::transform(mode.begin(), mode.end(), mode.begin(), ::tolower);
    std::transform(algo.begin(), algo.end(), algo.begin(), ::tolower);

    if (mode != "encrypt" && mode != "decrypt")
        return Utils::fatalError("Unknown mode '" + mode + "'. Use 'encrypt' or 'decrypt'.");

    if (algo != "aes" && algo != "xor")
        return Utils::fatalError("Unknown algorithm '" + algo + "'. Use 'aes' or 'xor'.");

    std::string passphrase = Utils::getPassphrase("  Enter passphrase: ");
    if (passphrase.empty())
        return Utils::fatalError("Passphrase must not be empty.");

    try {
        std::cout << "\033[1;37m  Processing...\033[0m\n";

        std::vector<uint8_t> inputData  = FileHandler::readFile(inPath);
        std::vector<uint8_t> outputData;

        bool encrypting = (mode == "encrypt");

        if (algo == "aes") {
            outputData = encrypting
                ? AesCipher::encrypt(inputData, passphrase)
                : AesCipher::decrypt(inputData, passphrase);
        } else {
            outputData = XorCipher::process(inputData, passphrase);
        }

        FileHandler::writeFile(outPath, outputData);

        std::cout
            << "\n  \033[1;32m[OK] " << (encrypting ? "Encrypted" : "Decrypted") << " successfully\033[0m\n"
            << "  \033[0;37mInput  :\033[0m " << inPath  << "  (" << inputData.size()  << " bytes)\n"
            << "  \033[0;37mOutput :\033[0m " << outPath << "  (" << outputData.size() << " bytes)\n"
            << "  \033[0;37mCipher :\033[0m " << (algo == "aes" ? "AES-128-CBC" : "XOR") << "\n\n";

    } catch (const std::exception& ex) {
        return Utils::fatalError(ex.what());
    }

    return 0;
}
