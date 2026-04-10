#!/bin/bash
# ========================================
# Code Coverage Testing Script for Linux/macOS
# ========================================

echo ""
echo "========================================"
echo "  Code Coverage Testing Tool"
echo "========================================"
echo ""

# Check if build directory exists
if [ ! -d "build" ]; then
    echo "[ERROR] Build directory not found!"
    echo "Please run: cmake -DENABLE_COVERAGE=ON -B build -S ."
    echo "Then run: cmake --build build"
    exit 1
fi

echo "Select coverage report type:"
echo "  1. HTML Report (detailed, with source code highlighting)"
echo "  2. Text Report (summary in console)"
echo "  3. Both Reports"
echo ""
read -p "Enter your choice (1/2/3): " choice

case $choice in
    1)
        echo ""
        echo "Generating HTML coverage report..."
        cmake --build build --target coverage
        if [ $? -eq 0 ]; then
            echo ""
            echo "Opening HTML report in browser..."
            # Try to open in default browser
            if command -v xdg-open &> /dev/null; then
                xdg-open build/coverage/index.html
            elif command -v open &> /dev/null; then
                open build/coverage/index.html
            else
                echo "Please open: build/coverage/index.html"
            fi
        fi
        ;;
    2)
        echo ""
        echo "Generating text coverage report..."
        cmake --build build --target coverage-text
        ;;
    3)
        echo ""
        echo "Generating both reports..."
        cmake --build build --target coverage-text
        cmake --build build --target coverage
        if [ $? -eq 0 ]; then
            echo ""
            echo "Opening HTML report in browser..."
            if command -v xdg-open &> /dev/null; then
                xdg-open build/coverage/index.html
            elif command -v open &> /dev/null; then
                open build/coverage/index.html
            else
                echo "Please open: build/coverage/index.html"
            fi
        fi
        ;;
    *)
        echo ""
        echo "[ERROR] Invalid choice!"
        exit 1
        ;;
esac

echo ""
echo "========================================"
echo "  Coverage testing completed!"
echo "========================================"
echo ""
