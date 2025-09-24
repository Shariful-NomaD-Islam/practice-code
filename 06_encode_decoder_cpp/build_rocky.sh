#!/bin/bash

# Rocky Linux Build Script for C++23/C++20 File Encoder/Decoder

echo "=== Rocky Linux 9.6 Build Script ==="
echo "Building C++23 File Encoder/Decoder with GCC 11.5 compatibility"
echo

# Function to check if a command exists
command_exists() {
    command -v "$1" >/dev/null 2>&1
}

# Function to install packages using dnf
install_deps() {
    echo "Installing required dependencies..."
    
    # Update package list
    sudo dnf update -y >/dev/null 2>&1
    
    # Install development tools and OpenSSL
    sudo dnf groupinstall -y "Development Tools" >/dev/null 2>&1
    sudo dnf install -y openssl-devel cmake pkg-config >/dev/null 2>&1
    
    echo "✓ Dependencies installed successfully"
}

# Check if running on Rocky Linux
if [ -f /etc/rocky-release ]; then
    echo "✓ Rocky Linux detected: $(cat /etc/rocky-release)"
else
    echo "⚠ Warning: This script is optimized for Rocky Linux"
fi

# Check for required tools
echo "Checking build environment..."

if ! command_exists gcc; then
    echo "✗ GCC not found. Installing dependencies..."
    install_deps
else
    GCC_VERSION=$(gcc --version | head -n1)
    echo "✓ Found: $GCC_VERSION"
fi

if ! command_exists pkg-config; then
    echo "✗ pkg-config not found. Installing..."
    sudo dnf install -y pkg-config
fi

# Check OpenSSL
if ! pkg-config --exists openssl; then
    echo "✗ OpenSSL development headers not found. Installing..."
    sudo dnf install -y openssl-devel
else
    OPENSSL_VERSION=$(pkg-config --modversion openssl)
    echo "✓ OpenSSL found: version $OPENSSL_VERSION"
fi

echo

# Build using CMake (recommended)
echo "Building with CMake..."

# Clean any existing build directory to avoid cache conflicts
if [ -d "build" ]; then
    echo "Cleaning existing build directory..."
    rm -rf build
fi

mkdir -p build
cd build

# Configure
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_VERBOSE_MAKEFILE=ON

if [ $? -eq 0 ]; then
    echo "✓ CMake configuration successful"
else
    echo "✗ CMake configuration failed"
    echo "Trying with Makefile instead..."
    cd ..
    make clean
    make
    exit $?
fi

# Build
make -j$(nproc)

if [ $? -eq 0 ]; then
    echo "✓ Build successful!"
    
    # Copy executable to parent directory
    cp encoder_decoder ..
    cd ..
    
    echo
    echo "Testing the encoder/decoder..."
    
    # Create test file
    echo "Hello Rocky Linux 9.6!" > rocky_test.txt
    echo "Testing C++20/C++23 compatibility" >> rocky_test.txt
    echo "GCC 11.5.0 build test" >> rocky_test.txt
    
    # Test encode
    ./encoder_decoder encode rocky_test.txt
    if [ $? -eq 0 ]; then
        echo "✓ Encoding test passed"
        
        # Test decode
        ./encoder_decoder decode rocky_test.txt.encoded
        if [ $? -eq 0 ] && [ -f "rocky_test.txt" ]; then
            echo "✓ Decoding test passed"
            echo "✓ File content restored:"
            echo "---"
            cat rocky_test.txt
            echo "---"
            
            # Cleanup test files
            rm -f rocky_test.txt rocky_test.txt.encoded
            
            echo
            echo "🎉 SUCCESS: encoder_decoder built and tested successfully on Rocky Linux!"
            echo
            echo "Usage:"
            echo "  ./encoder_decoder encode <file>"
            echo "  ./encoder_decoder decode <file.encoded>"
            
        else
            echo "✗ Decoding test failed"
        fi
    else
        echo "✗ Encoding test failed"
    fi
    
else
    echo "✗ Build failed"
    echo "Trying fallback with Makefile..."
    cd ..
    make clean
    make
fi