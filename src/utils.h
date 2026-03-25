#pragma once
#include <string>

#define FENC_VERSION  "1.0.0"
#define FENC_NAME     "FileEncrypt"

namespace Utils {
    void printBanner();

    /** passphrase (masked on Windows). */
    std::string getPassphrase(const std::string& prompt);

    /** Print a formatted error message and return exit code 1. */
    int fatalError(const std::string& msg);

    void printHelp(const std::string& programName);
}
