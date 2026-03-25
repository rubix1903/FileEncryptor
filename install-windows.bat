@echo off
setlocal enabledelayedexpansion

:: ─────────────────────────────────────────────────────────────
::  fencrypt  —  Windows Installer (cmd / PowerShell)
::  installs fencrypt to PATH
:: ─────────────────────────────────────────────────────────────

echo.
echo   [36m███████╗███╗   ██╗ ██████╗██████╗ ██╗   ██╗██████╗ ████████╗[0m
echo   [36m██╔════╝████╗  ██║██╔════╝██╔══██╗╚██╗ ██╔╝██╔══██╗╚══██╔══╝[0m
echo   [36m█████╗  ██╔██╗ ██║██║     ██████╔╝ ╚████╔╝ ██████╔╝   ██║   [0m
echo   [36m██╔══╝  ██║╚██╗██║██║     ██╔══██╗  ╚██╔╝  ██╔═══╝    ██║   [0m
echo   [36m███████╗██║ ╚████║╚██████╗██║  ██║   ██║   ██║        ██║   [0m
echo   [36m╚══════╝╚═╝  ╚═══╝ ╚═════╝╚═╝  ╚═╝   ╚═╝   ╚═╝        ╚═╝   [0m
echo.
echo   [33m  FileEncrypt v1.0.0 — Windows Installer[0m
echo     ──────────────────────────────────────────
echo.

set INSTALL_DIR=%ProgramFiles%\fencrypt
set BUILD_DIR=%~dp0_build_release

:: Step 1 — Check cmake
echo [36m[1/4] Checking dependencies...[0m
where cmake >nul 2>&1
if %errorlevel% neq 0 (
    echo [31m[ERROR][0m cmake not found.
    echo   Install CMake from: https://cmake.org/download/
    echo   Ensure you tick "Add CMake to system PATH" during install.
    exit /b 1
)
where cl >nul 2>&1
if %errorlevel% neq 0 (
    where g++ >nul 2>&1
    if !errorlevel! neq 0 (
        echo [31m[ERROR][0m No C++ compiler found.
        echo   Install Visual Studio Build Tools: https://aka.ms/vs/buildtools
        echo   Or install MinGW-w64 via: https://www.mingw-w64.org/
        exit /b 1
    )
)
echo   [32m✔ cmake and C++ compiler found[0m

:: Step 2 — Build
echo [36m[2/4] Building release binary...[0m
if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"
cmake -S "%~dp0" -B "%BUILD_DIR%" -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX="%INSTALL_DIR%" >nul
cmake --build "%BUILD_DIR%" --config Release
if %errorlevel% neq 0 (
    echo [31m[ERROR][0m Build failed. Check compiler output above.
    exit /b 1
)
echo   [32m✔ Build successful[0m

:: Step 3 — Install
echo [36m[3/4] Installing to %INSTALL_DIR%...[0m
if not exist "%INSTALL_DIR%" mkdir "%INSTALL_DIR%"
cmake --install "%BUILD_DIR%" --config Release >nul
echo   [32m✔ Installed to %INSTALL_DIR%\fencrypt.exe[0m

:: Step 4 — Add to PATH via registry
echo [36m[4/4] Adding to system PATH...[0m
setx PATH "%INSTALL_DIR%;%PATH%" /M >nul 2>&1
if %errorlevel% neq 0 (
    echo   [33m⚠ Could not update system PATH automatically.[0m
    echo     Add manually: %INSTALL_DIR%
) else (
    echo   [32m✔ Added %INSTALL_DIR% to system PATH[0m
)

echo.
echo   [32mInstallation complete![0m
echo     ──────────────────────────────────────────
echo     Open a NEW terminal and run:
echo       fencrypt --help
echo       fencrypt encrypt aes myfile.txt myfile.enc
echo.
pause
