@echo off
REM ========================================
REM Code Coverage Testing Script for Windows
REM ========================================

echo.
echo ========================================
echo   Code Coverage Testing Tool
echo ========================================
echo.

REM Check if build directory exists
if not exist "build" (
    echo [ERROR] Build directory not found!
    echo Please run: cmake -DENABLE_COVERAGE=ON -G "MinGW Makefiles" -B build -S .
    echo Then run: cmake --build build
    exit /b 1
)

echo Select coverage report type:
echo   1. HTML Report (detailed, with source code highlighting)
echo   2. Text Report (summary in console)
echo   3. Both Reports
echo.
set /p choice="Enter your choice (1/2/3): "

if "%choice%"=="1" (
    echo.
    echo Generating HTML coverage report...
    cmake --build build --target coverage
    if %errorlevel% equ 0 (
        echo.
        echo Opening HTML report in browser...
        start build\coverage\index.html
    )
) else if "%choice%"=="2" (
    echo.
    echo Generating text coverage report...
    cmake --build build --target coverage-text
) else if "%choice%"=="3" (
    echo.
    echo Generating both reports...
    cmake --build build --target coverage-text
    cmake --build build --target coverage
    if %errorlevel% equ 0 (
        echo.
        echo Opening HTML report in browser...
        start build\coverage\index.html
    )
) else (
    echo.
    echo [ERROR] Invalid choice!
    exit /b 1
)

echo.
echo ========================================
echo   Coverage testing completed!
echo ========================================
echo.
