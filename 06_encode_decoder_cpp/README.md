# C++23 File Encoder/Decoder

A modern C++23 command-line file encoder/decoder using AES-256-CBC encryption with full Unicode support.

## Features

- 🔐 **AES-256-CBC Encryption**: Military-grade encryption with PBKDF2 key derivation
- 🌍 **Unicode Support**: Full UTF-8 character support including emojis and special characters
- 🔑 **Secure Password**: PBKDF2 with 100,000 iterations and random salt
- 🗂️ **File Management**: Automatically deletes original files after encoding
- 📝 **Binary Safe**: Custom file format with magic header and integrity checks
- 🎨 **Modern C++23**: Uses latest C++ features like `std::format`

## Requirements

- **C++23 compatible compiler** (GCC 11+ or Clang 14+)
- **OpenSSL 3.x**
- **pkg-config** (for dependency management)
- **macOS/Linux** (Windows support with MinGW)

## Installation

### macOS (using Homebrew)

```bash
# Install dependencies
brew install openssl pkg-config

# Clone and build
git clone <repository>
cd 06_encode_decoder_cpp
./build.sh
```

### Manual Build

```bash
# Using Makefile
make install-deps  # Install dependencies
make              # Build

# Using CMake
mkdir build && cd build
cmake ..
make
```

## Usage

### Basic Commands

```bash
# Encode a file (original file will be deleted)
./encoder_decoder encode sample.txt

# Decode an encoded file
./encoder_decoder decode sample.txt.encoded
```

### With Password

```bash
# Provide password as argument
./encoder_decoder encode sample.txt mypassword123
./encoder_decoder decode sample.txt.encoded mypassword123

# Or be prompted for password (more secure)
./encoder_decoder encode sample.txt
./encoder_decoder decode sample.txt.encoded
```

## Example Usage

### Encoding a File

```bash
$ ./encoder_decoder encode sample.txt mypassword
=== C++23 File Encoder/Decoder ===
Using AES-256-CBC encryption with PBKDF2 key derivation

Original content: ._✗_.I want to be decoded.._✗_.
Content size: 33 bytes
✓ File encoded successfully: sample.txt.encoded
✓ Original file 'sample.txt' deleted
Encrypted size: 48 bytes

✓ Encoding completed successfully!
```

### Decoding a File

```bash
$ ./encoder_decoder decode sample.txt.encoded mypassword
=== C++23 File Encoder/Decoder ===
Using AES-256-CBC encryption with PBKDF2 key derivation

Encrypted data size: 48 bytes

==================================================
DECODED CONTENT:
==================================================
._✗_.I want to be decoded.._✗_.
==================================================
Decoded size: 33 bytes

✓ Decoding completed successfully!
```

## Security Features

### Encryption Details

- **Algorithm**: AES-256-CBC (Advanced Encryption Standard)
- **Key Derivation**: PBKDF2 with SHA-256
- **Iterations**: 100,000 (OWASP recommended minimum)
- **Salt**: 16 bytes random salt per file
- **IV**: 16 bytes random initialization vector per file

### File Format

```
Magic Header: "ENC23" (5 bytes)
Salt:         16 bytes (random)
IV:           16 bytes (random)
Data Size:    4 bytes (uint32_t)
Encrypted:    Variable length
```

## Unicode Support

The encoder/decoder fully supports Unicode characters:

```bash
# Create a file with Unicode content
echo "Hello 🌍 世界 🔐 Encryption! ñáéíóú" > unicode.txt

# Encode it
./encoder_decoder encode unicode.txt mypass

# Decode it
./encoder_decoder decode unicode.txt.encoded mypass
```

## Testing

```bash
# Run complete test cycle
make test

# Test encoding only
make test-encode

# Test decoding only
make test-decode
```

## File Structure

```
06_encode_decoder_cpp/
├── main.cpp           # Main application code
├── CMakeLists.txt     # CMake build configuration
├── Makefile           # Make build configuration
├── build.sh           # Automated build script
├── README.md          # This file
└── sample.txt         # Sample text file for testing
```

## Error Handling

The application provides detailed error messages:

- **File not found**: Clear indication when input files don't exist
- **Wrong password**: Decryption fails gracefully with error message
- **Corrupted files**: Magic header validation prevents invalid file processing
- **Permission errors**: Clear messages for file access issues

## Troubleshooting

### OpenSSL Not Found
```bash
# macOS
brew install openssl
export PKG_CONFIG_PATH="/opt/homebrew/opt/openssl/lib/pkgconfig:$PKG_CONFIG_PATH"

# Ubuntu/Debian
sudo apt-get install libssl-dev

# Arch Linux
sudo pacman -S openssl
```

### Compiler Issues
```bash
# Check C++23 support
g++ --version

# Use newer compiler if needed
brew install gcc  # macOS
```

### Build Errors
```bash
# Clean and rebuild
make clean
make

# Or with CMake
rm -rf build
mkdir build && cd build
cmake ..
make
```

## Security Considerations

1. **Password Storage**: Passwords are not stored anywhere - only you know them
2. **File Deletion**: Original files are securely deleted after encoding
3. **Random Generation**: Uses OpenSSL's cryptographically secure random generator
4. **Memory Safety**: Modern C++ RAII principles prevent memory leaks

## License

MIT License - Feel free to use and modify as needed.

## Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Test thoroughly with various file types and Unicode content
5. Submit a pull request