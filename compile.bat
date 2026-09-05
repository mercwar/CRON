:: AVIS Neon Core - MSVC Build Script
:: AIFVS-ARTIFACT
:: Filename: compile_main.bat
:: Purpose: Compile all sources into CRON.exe using MSVC

@echo off
setlocal EnableExtensions
title CRON - MSVC Build Engine

:: Workspace variables - Updated targeting name structure to CRON
set "EXE_OUT=%~dp0CRON.exe"

echo ============================================
echo   CRON ENGINE CORE :: BUILD PROJECT
echo ============================================
echo.

:: --- Initialize MSVC environment ---
call "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars64.bat"

:: --- Change to project directory ---
cd /d "%~dp0"

:: --- Clean old outputs ---
if exist "%EXE_OUT%" del /f /q "%EXE_OUT%"
if exist "%~dp0main.exe" del /f /q "%~dp0main.exe"
del /f /q *.obj 2>nul
del /f /q *.res 2>nul

:: --- Compile resources and main application files via MSVC toolset ---
rc.exe resource.rc

cl.exe /O2 /W4 /D_CRT_SECURE_NO_WARNINGS ^
    main.c ^
    avis_window.c ^
    cdata_loader.c ^
    resource.res ^
    /Fe:"%EXE_OUT%" ^
    /link /SUBSYSTEM:WINDOWS user32.lib gdi32.lib shell32.lib


if %errorlevel% EQU 0 (
    echo.
    echo ============================================
    echo   CRON BUILD COMPLETE: SUCCESS
    echo   Output Binary: %EXE_OUT%
    echo ============================================
    del /f /q *.obj 2>nul
    del /f /q *.res 2>nul
) else (
    echo.
    echo [CRON] CRITICAL ERROR: Compilation failed.
)

echo.
pause
