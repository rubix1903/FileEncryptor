#!/usr/bin/env bash
# ─────────────────────────────────────────────────────────────
#  fencrypt  —  macOS installer
#  installs to /usr/local/bin/fencrypt
# ─────────────────────────────────────────────────────────────
set -e

RED='\033[1;31m'; GRN='\033[1;32m'; CYN='\033[1;36m'; YLW='\033[1;33m'; RST='\033[0m'

echo -e "${CYN}"
echo "  ███████╗███╗   ██╗ ██████╗██████╗ ██╗   ██╗██████╗ ████████╗"
echo "  ██╔════╝████╗  ██║██╔════╝██╔══██╗╚██╗ ██╔╝██╔══██╗╚══██╔══╝"
echo "  █████╗  ██╔██╗ ██║██║     ██████╔╝ ╚████╔╝ ██████╔╝   ██║   "
echo "  ██╔══╝  ██║╚██╗██║██║     ██╔══██╗  ╚██╔╝  ██╔═══╝    ██║   "
echo "  ███████╗██║ ╚████║╚██████╗██║  ██║   ██║   ██║        ██║   "
echo "  ╚══════╝╚═╝  ╚═══╝ ╚═════╝╚═╝  ╚═╝   ╚═╝   ╚═╝        ╚═╝   "
echo -e "${RST}"
echo -e "${YLW}  FileEncrypt v1.0.0 — macOS Installer${RST}"
echo    "  ──────────────────────────────────────"
echo ""

INSTALL_DIR="/usr/local/bin"
BUILD_DIR="$(pwd)/_build_release"

# Check for Xcode CLT or Homebrew cmake
echo -e "${CYN}[1/4] Checking dependencies...${RST}"
if ! command -v cmake &>/dev/null; then
    if command -v brew &>/dev/null; then
        echo "  cmake not found. Installing via Homebrew..."
        brew install cmake
    else
        echo -e "${RED}[ERROR]${RST} cmake not found."
        echo "  Install Homebrew: https://brew.sh  then run: brew install cmake"
        exit 1
    fi
fi
if ! command -v clang++ &>/dev/null && ! command -v g++ &>/dev/null; then
    echo -e "${RED}[ERROR]${RST} No C++ compiler found."
    echo "  Run: xcode-select --install"
    exit 1
fi
echo -e "${GRN}  ✔ cmake and C++ compiler found${RST}"

# Build
echo -e "${CYN}[2/4] Building release binary...${RST}"
mkdir -p "$BUILD_DIR"
cmake -S . -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local > /dev/null
cmake --build "$BUILD_DIR" --config Release -j"$(sysctl -n hw.logicalcpu)"
echo -e "${GRN}  ✔ Build successful${RST}"

# Install
echo -e "${CYN}[3/4] Installing to ${INSTALL_DIR}...${RST}"
if [ "$EUID" -ne 0 ]; then
    echo "  (sudo required for /usr/local/bin)"
    sudo cmake --install "$BUILD_DIR" > /dev/null
else
    cmake --install "$BUILD_DIR" > /dev/null
fi
echo -e "${GRN}  ✔ Installed to ${INSTALL_DIR}/fencrypt${RST}"

# Verify
echo -e "${CYN}[4/4] Verifying installation...${RST}"
if command -v fencrypt &>/dev/null; then
    echo -e "${GRN}  ✔ fencrypt is available in PATH${RST}"
else
    echo -e "${YLW}  ⚠ Add ${INSTALL_DIR} to your PATH in ~/.zshrc or ~/.bash_profile${RST}"
fi

echo ""
echo -e "${GRN}  Installation complete!${RST}"
echo    "  ──────────────────────────────────────"
echo    "  Run:  fencrypt --help"
echo    "        fencrypt encrypt aes myfile.txt myfile.enc"
echo ""
