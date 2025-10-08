#!/bin/bash

# Build the C++ library
echo "Building C++ library..."
cd cpp
if [ -d "build" ]; then
    echo "Removing existing build directory..."
    rm -rf build
fi
mkdir -p build
cd build
cmake ..
cmake --build .
cd ../..

# Build the Rust project in release mode (production)
echo "Building Rust project in release mode..."
cargo build --release

echo "Build complete!"

# Run the production executable
echo "Running the production executable..."
./target/release/rust_with_cpp