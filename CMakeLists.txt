cmake_minimum_required(VERSION 3.20)
project(FileEncryptor VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(src)

set(SOURCES
    src/main.cpp
    src/xor_cipher.cpp
    src/aes_cipher.cpp
    src/file_handler.cpp
    src/utils.cpp
)

add_executable(fencrypt ${SOURCES})

target_compile_options(fencrypt PRIVATE
    $<$<AND:$<CXX_COMPILER_ID:GNU,Clang>,$<CONFIG:Release>>:-O3 -DNDEBUG>
    $<$<AND:$<CXX_COMPILER_ID:GNU,Clang>,$<CONFIG:Debug>>:-O0 -g -Wall -Wextra>
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /O2>
)

# ── Install target (used by all platform installers) ──────────────────────────
include(GNUInstallDirs)
install(TARGETS fencrypt
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# ── CPack – cross-platform packaging ─────────────────────────────────────────
set(CPACK_PACKAGE_NAME              "fencrypt")
set(CPACK_PACKAGE_VENDOR            "FileEncrypt Project")
set(CPACK_PACKAGE_DESCRIPTION_SHORT "AES-128-CBC & XOR file encryption CLI")
set(CPACK_PACKAGE_VERSION           "1.0.0")
set(CPACK_PACKAGE_VERSION_MAJOR     "1")
set(CPACK_PACKAGE_VERSION_MINOR     "0")
set(CPACK_PACKAGE_VERSION_PATCH     "0")
set(CPACK_RESOURCE_FILE_LICENSE     "${CMAKE_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README      "${CMAKE_SOURCE_DIR}/README.md")

# DEB (Linux)
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "you@example.com")
set(CPACK_DEBIAN_PACKAGE_SECTION    "utils")
set(CPACK_DEBIAN_PACKAGE_DEPENDS    "libc6 (>= 2.17), libstdc++6 (>= 9)")

# RPM (Linux)
set(CPACK_RPM_PACKAGE_LICENSE       "MIT")
set(CPACK_RPM_PACKAGE_GROUP         "Applications/System")

# NSIS (Windows)
set(CPACK_NSIS_DISPLAY_NAME         "FileEncrypt ${CPACK_PACKAGE_VERSION}")
set(CPACK_NSIS_MODIFY_PATH          ON)

include(CPack)
