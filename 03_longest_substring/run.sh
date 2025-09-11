#!/bin/bash

# Longest Unrepeated Substring Compilation and Execution Script
# This script compiles and runs the Longest Unrepeated Substring example

echo "=== Longest unrepeated substring Build Script ==="
echo ""

# Creating out folder if not exists
mkdir -p out

# Clean up any existing executable
echo "Cleaning up previous builds..."
if [ -f "out/process" ]; then
    rm out/process
    echo "Removed existing executable"
fi

echo ""
echo "Compiling main.cpp..."

# Compile with C++20 standard and pthread support
g++ -std=c++23 -pthread -Wall -Wextra -O2 main.cpp -o out/process

# Check if compilation was successful
if [ $? -eq 0 ]; then
    echo "✅ Compilation successful!"
    echo ""
    echo "Running Longest Unrepeated Substring example..."
    echo "================================="
    ./out/process
    echo "================================="
    echo "✅ Execution completed!"
else
    echo "❌ Compilation failed!"
    exit 1
fi

echo ""
echo "Build script finished."
