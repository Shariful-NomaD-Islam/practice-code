#!/bin/bash

# ThreadSafe Cache Compilation and Execution Script
# This script compiles and runs the ThreadSafeCache example

echo "=== ThreadSafe Cache Build Script ==="
echo ""

# Creating out folder if not exists
mkdir -p out

# Clean up any existing executable
echo "Cleaning up previous builds..."
if [ -f "out/ThreadSafeCache_Main" ]; then
    rm out/ThreadSafeCache_Main
    echo "Removed existing executable"
fi

echo ""
echo "Compiling ThreadSafeCache_Main.cpp..."

# Compile with C++20 standard and pthread support
g++ -std=c++23 -pthread -Wall -Wextra -O2 ThreadSafeCache_Main.cpp -o out/ThreadSafeCache_Main

# Check if compilation was successful
if [ $? -eq 0 ]; then
    echo "✅ Compilation successful!"
    echo ""
    echo "Running ThreadSafeCache example..."
    echo "================================="
    ./out/ThreadSafeCache_Main
    echo "================================="
    echo "✅ Execution completed!"
else
    echo "❌ Compilation failed!"
    exit 1
fi

echo ""
echo "Build script finished."
