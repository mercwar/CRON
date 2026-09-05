:: AVIS Neon Core - Kernel Transmitter Compiler (Batch 2)
:: AIFVS-ARTIFACT
:: Filename: kernel_parse.bat
:: Purpose: Parse WIN11_COMMANDS.INI into command=description objects for CDATA

@echo off
setlocal EnableExtensions EnableDelayedExpansion
title Kernel Transmitter - INI Parser

rem Define input INI file
set "INI_FILE=%~dp0WIN11_COMMANDS.INI"
set "OBJ_FILE=%~dp0WIN11_OBJECTS.CDATA"

echo ============================================
echo   KERNEL TRANSMITTER :: PARSING INI TO OBJECT
echo   ESTIMATED TIME :: 120 seconds
echo ============================================
echo.

if not exist "%INI_FILE%" (
    echo [AVIS] CRITICAL ERROR: INI FILE NOT FOUND
    pause
    exit /b 1
)

> "%OBJ_FILE%" echo [objects]

rem Read INI line by line, using findstr up front to filter out section headers
rem This completely preserves special characters without parsing syntax errors
for /f "usebackq delims=" %%A in (`findstr /v /r "^\[" "%INI_FILE%"`) do (
    set "line=%%A"
    
    if defined line (
        rem Safely evaluate if line contains an equal sign
        echo !line!| findstr "=" >nul
        if !errorlevel! EQU 0 (
            echo !line!>>"%OBJ_FILE%"
        ) else (
            for /f "tokens=1,* delims= " %%X in ("!line!") do (
                set "key=%%X"
                set "val=%%Y"
                if defined val (
                    echo !key!=!val!>>"%OBJ_FILE%"
                ) else (
                    echo !key!=>>"%OBJ_FILE%"
                )
            )
        )
    )
)

echo.
echo [AVIS] OBJECT FILE GENERATED:
echo   %OBJ_FILE%
echo.
pause
