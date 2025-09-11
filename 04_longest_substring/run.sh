#!/bin/bash

# This script compiles and runs the longest_unique_substring_without_repeating_character example

echo "=== Longest Unique Substring Without Repeating Characters Build Script ==="
echo ""

# Create out folder
mkdir -p out

# Clean up any existing executable and target folder
echo "Cleaning up previous builds..."
# Remove target folder (Rust build artifacts)
if [ -d "target" ]; then
    rm -rf target
    echo "Removed target folder"
fi

# Remove target previous compiled executables
if [ -d "./out/execute" ]; then
    rm ./out/execute
    echo "Removed previous executable"
fi

echo ""
echo "Compiling main.rs ..."

# Compile with rustc
rustc src/main.rs -o ./out/execute

# Check if compilation was successful
if [ $? -eq 0 ]; then
    echo "✅ Compilation successful!"
    echo ""
    echo "Running longest_unique_substring_without_repeating_character example..."
    echo "================================="
    ./out/execute
    echo "================================="
    echo "✅ Execution completed!"
else
    echo "❌ Compilation failed!"
    exit 1
fi

echo ""
echo "Build script finished."

echo "You could have run using command 'cargo run'"