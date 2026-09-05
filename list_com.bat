:: AVIS Neon Core - Kernel Transmitter Compiler (Batch 1)
:: AIFVS-ARTIFACT
:: Filename: kernel_cmd.bat
:: Purpose: Enumerate all available commands and executables, output to Win11 INI file

@echo off
setlocal EnableExtensions EnableDelayedExpansion
title Kernel Transmitter - Command Catalog Compiler

:: Define output INI file
set "INI_FILE=%~dp0WIN11_COMMANDS.INI"

echo [commands] > "%INI_FILE%"
echo ============================================ 
echo   KERNEL TRANSMITTER :: BUILDING WIN11 INI
echo   ESTIMATED TIME :: 120 seconds
echo ============================================ 
echo.

:: --- Internal CMD commands ---
for /f "tokens=*" %%C in ('help') do (
    echo %%C>>"%INI_FILE%"
)

:: --- System32 executables ---
echo [system32]>>"%INI_FILE%"
for %%F in (C:\Windows\System32\*.exe) do (
    echo %%~nxF>>"%INI_FILE%"
)

:: --- PATH executables (hardened) ---
echo [path_executables]>>"%INI_FILE%"
for %%P in (%PATH:;= %) do (
    if exist "%%P" (
        for %%X in ("%%P\*.exe" "%%P\*.bat") do (
            if exist %%X echo %%~nxX>>"%INI_FILE%"
        )
    )
)

echo.
echo [AVIS] WIN11 INI FILE GENERATED:
echo   %INI_FILE%
echo.
pause
