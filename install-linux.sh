#!/usr/bin/env bash
# ─────────────────────────────────────────────────────────────
#  fencrypt  —  Linux installer
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
echo -e "${YLW}  FileEncrypt v1.0.0 — Linux Installer${RST}"
echo    "  ──────────────────────────────────────"
echo ""

INSTALL_DIR="/usr/local/bin"
BUILD_DIR="$(pwd)/_build_release"

# Check dependencies
echo -e "${CYN}[1/4] Checking dependencies...${RST}"
for dep in cmake g++ make; do
    if ! command -v "$dep" &>/dev/null; then
        echo -e "${RED}[ERROR]${RST} '$dep' not found. Install it with:"
        echo "        sudo apt install cmake g++ make     # Debian/Ubuntu"
        echo "        sudo dnf install cmake gcc-c++ make # Fedora/RHEL"
        exit 1
    fi
done
echo -e "${GRN}  ✔ cmake, g++, make found${RST}"

# Build
echo -e "${CYN}[2/4] Building release binary...${RST}"
mkdir -p "$BUILD_DIR"
cmake -S . -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local > /dev/null
cmake --build "$BUILD_DIR" --config Release -j"$(nproc)"
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
    echo -e "${YLW}  ⚠ fencrypt not in PATH — add ${INSTALL_DIR} to your PATH${RST}"
fi

echo ""
echo -e "${GRN}  Installation complete!${RST}"
echo    "  ──────────────────────────────────────"
echo    "  Run:  fencrypt --help"
echo    "        fencrypt encrypt aes myfile.txt myfile.enc"
echo ""
