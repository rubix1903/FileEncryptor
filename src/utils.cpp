#include "utils.h"
#include <iostream>
#include <string>

#ifdef _WIN32
  #include <windows.h>
#else
  #include <termios.h>
  #include <unistd.h>
#endif

namespace Utils {

void printBanner() {

#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif

    std::cout <<
    "\n\033[1;36m"
    "  ███████╗███╗   ██╗ ██████╗██████╗ ██╗   ██╗██████╗ ████████╗\n"
    "  ██╔════╝████╗  ██║██╔════╝██╔══██╗╚██╗ ██╔╝██╔══██╗╚══██╔══╝\n"
    "  █████╗  ██╔██╗ ██║██║     ██████╔╝ ╚████╔╝ ██████╔╝   ██║   \n"
    "  ██╔══╝  ██║╚██╗██║██║     ██╔══██╗  ╚██╔╝  ██╔═══╝    ██║   \n"
    "  ███████╗██║ ╚████║╚██████╗██║  ██║   ██║   ██║        ██║   \n"
    "  ╚══════╝╚═╝  ╚═══╝ ╚═════╝╚═╝  ╚═╝   ╚═╝   ╚═╝        ╚═╝   \n"
    "\033[0m"
    "\033[1;37m"
    "  ──────────────────────────────────────────────────────────────\n"
    "\033[0;33m"
    "    File Encryption Utility  |  AES-128-CBC & XOR  |  v" FENC_VERSION "\n"
    "\033[1;37m"
    "  ──────────────────────────────────────────────────────────────\n"
    "\033[0m\n";
}

std::string getPassphrase(const std::string& prompt) {
    std::cout << "\033[1;33m" << prompt << "\033[0m" << std::flush;
    std::string pass;

#ifdef _WIN32
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode;
    GetConsoleMode(hStdin, &mode);
    SetConsoleMode(hStdin, mode & ~ENABLE_ECHO_INPUT);
    std::getline(std::cin, pass);
    SetConsoleMode(hStdin, mode);
#else
    termios oldt{}, newt{};
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~static_cast<tcflag_t>(ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    std::getline(std::cin, pass);
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
#endif

    std::cout << '\n';
    return pass;
}

int fatalError(const std::string& msg) {
    std::cerr << "\033[1;31m[ERROR]\033[0m " << msg << '\n';
    return 1;
}

void printHelp(const std::string& prog) {
    std::cout
        << "\033[1;36mUsage:\033[0m\n"
        << "  " << prog << " <mode> <algorithm> <input_file> <output_file>\n\n"
        << "\033[1;36mModes:\033[0m\n"
        << "  \033[1;33mencrypt\033[0m   Encrypt the input file\n"
        << "  \033[1;33mdecrypt\033[0m   Decrypt the input file\n\n"
        << "\033[1;36mAlgorithms:\033[0m\n"
        << "  \033[1;33maes\033[0m       AES-128-CBC  (recommended, secure)\n"
        << "  \033[1;33mxor\033[0m       XOR cipher   (fast, lightweight)\n\n"
        << "\033[1;36mExamples:\033[0m\n"
        << "  " << prog << " encrypt aes  secret.txt  secret.enc\n"
        << "  " << prog << " decrypt aes  secret.enc  secret.txt\n"
        << "  " << prog << " encrypt xor  photo.jpg   photo.xor\n"
        << "  " << prog << " decrypt xor  photo.xor   photo.jpg\n\n"
        << "\033[1;36mNotes:\033[0m\n"
        << "  - Passphrase input is hidden (not echoed to terminal)\n"
        << "  - AES uses CBC mode with a random IV and PKCS#7 padding\n"
        << "  - XOR is fast but less secure; use AES for sensitive data\n"
        << "  - Run \033[1;33m" << prog << " --version\033[0m to see version info\n\n";
}

} // namespace Utils
