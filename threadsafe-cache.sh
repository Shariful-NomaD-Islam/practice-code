#!/bin/bash

# ThreadSafe Cache Compilation and Execution Script
# This script compiles and runs the ThreadSafeCache example

echo "=== ThreadSafe Cache Build Script ==="
echo ""

# Clean up any existing executable
echo "Cleaning up previous builds..."
if [ -f "ThreadSafeCache_Main" ]; then
    rm ThreadSafeCache_Main
    echo "Removed existing executable"
fi

echo ""
echo "Compiling ThreadSafeCache_Main.cpp..."

# Compile with C++20 standard and pthread support
g++ -std=c++23 -pthread -Wall -Wextra -O2 ThreadSafeCache_Main.cpp -o ThreadSafeCache_Main

# Check if compilation was successful
if [ $? -eq 0 ]; then
    echo "✅ Compilation successful!"
    echo ""
    echo "Running ThreadSafeCache example..."
    echo "================================="
    ./ThreadSafeCache_Main
    echo "================================="
    echo "✅ Execution completed!"
else
    echo "❌ Compilation failed!"
    exit 1
fi

echo ""
echo "Build script finished."
