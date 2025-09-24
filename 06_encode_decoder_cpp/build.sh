#!/bin/bash

# C++23 File Encoder/Decoder Build Script

echo "=== C++23 File Encoder/Decoder Build Script ==="
echo

# Check if OpenSSL is installed
check_openssl() {
    if pkg-config --exists openssl; then
        echo "✓ OpenSSL found: $(pkg-config --modversion openssl)"
        return 0
    else
        echo "✗ OpenSSL not found"
        return 1
    fi
}

# Install dependencies on macOS
install_dependencies() {
    echo "Installing dependencies..."
    
    if command -v brew >/dev/null 2>&1; then
        echo "Installing OpenSSL via Homebrew..."
        brew install openssl pkg-config
        
        # Add OpenSSL to PKG_CONFIG_PATH if needed
        if [[ "$OSTYPE" == "darwin"* ]]; then
            export PKG_CONFIG_PATH="/opt/homebrew/opt/openssl/lib/pkgconfig:$PKG_CONFIG_PATH"
            echo "Added OpenSSL to PKG_CONFIG_PATH"
        fi
    else
        echo "Homebrew not found. Please install:"
        echo "  /bin/bash -c \"\$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)\""
        exit 1
    fi
}

# Build using Makefile
build_with_make() {
    echo "Building with Makefile..."
    make clean
    make
}

# Build using CMake
build_with_cmake() {
    echo "Building with CMake..."
    mkdir -p build
    cd build
    cmake ..
    make
    cd ..
    cp build/encoder_decoder .
}

# Main build process
main() {
    # Check for C++23 compatible compiler
    if ! command -v g++ >/dev/null 2>&1; then
        echo "Error: No C++ compiler found!"
        exit 1
    fi
    
    # Check compiler version
    GCC_VERSION=$(g++ --version | head -n1 | grep -o '[0-9]\+' | head -1)
    if [ "$GCC_VERSION" -lt 11 ]; then
        echo "Warning: GCC version $GCC_VERSION may not support C++23"
    fi
    
    # Set PKG_CONFIG_PATH for macOS OpenSSL
    if [[ "$OSTYPE" == "darwin"* ]]; then
        export PKG_CONFIG_PATH="/opt/homebrew/opt/openssl/lib/pkgconfig:/usr/local/opt/openssl/lib/pkgconfig:$PKG_CONFIG_PATH"
    fi
    
    # Check OpenSSL
    if ! check_openssl; then
        echo
        read -p "Install OpenSSL dependencies? (y/n): " -n 1 -r
        echo
        if [[ $REPLY =~ ^[Yy]$ ]]; then
            install_dependencies
        else
            echo "OpenSSL is required to build this project."
            exit 1
        fi
    fi
    
    # Choose build method
    echo
    echo "Choose build method:"
    echo "1) Makefile (default)"
    echo "2) CMake"
    read -p "Enter choice (1-2): " choice
    
    case $choice in
        2)
            build_with_cmake
            ;;
        *)
            build_with_make
            ;;
    esac
    
    # Check if build was successful
    if [ -f "encoder_decoder" ]; then
        echo
        echo "✓ Build successful!"
        echo
        echo "Usage:"
        echo "  ./encoder_decoder encode sample.txt"
        echo "  ./encoder_decoder decode sample.txt.encoded"
        echo
        
        # Test with sample file if it exists
        if [ -f "sample.txt" ]; then
            echo "Sample file found. You can test with:"
            echo "  ./encoder_decoder encode sample.txt"
            echo "  ./encoder_decoder decode sample.txt.encoded"
            echo
            echo "Or run a complete test with:"
            echo "  make test"
        fi
    else
        echo "✗ Build failed!"
        exit 1
    fi
}

# Run main function
main "$@"