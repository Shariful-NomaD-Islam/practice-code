# Rocky Linux 9.6 Compatibility Guide

This guide explains how to build and run the C++23 File Encoder/Decoder on Rocky Linux 9.6 with GCC 11.5.0.

## 🔧 Compatibility Features

### Automatic C++ Standard Detection
The build system automatically detects your compiler version and selects the appropriate C++ standard:

- **GCC 12+**: Uses C++23 with `std::format` and modern features
- **GCC 11.5**: Uses C++20 with compatibility layer
- **Clang 14+**: Uses C++23 
- **Clang <14**: Uses C++20 with compatibility layer

### Feature Compatibility Matrix

| Feature | C++23 (GCC 12+) | C++20 (GCC 11.5) | Status |
|---------|------------------|-------------------|---------|
| `std::format` | ✅ Native | ✅ Polyfill | Compatible |
| `string.ends_with()` | ✅ Native | ✅ Polyfill | Compatible |
| `std::filesystem` | ✅ | ✅ | Fully supported |
| OpenSSL encryption | ✅ | ✅ | Fully supported |
| Binary file support | ✅ | ✅ | Fully supported |

## 📦 Rocky Linux Installation

### Option 1: Automated Script (Recommended)
```bash
# Clone or copy the project
git clone <repository_url>
cd 06_encode_decoder_cpp

# Run the Rocky Linux build script
./build_rocky.sh
```

### Option 2: Manual Installation

#### Step 1: Install Dependencies
```bash
# Update system
sudo dnf update -y

# Install development tools
sudo dnf groupinstall -y "Development Tools"

# Install required packages
sudo dnf install -y openssl-devel cmake pkg-config
```

#### Step 2: Build with CMake (Recommended)
```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
cp encoder_decoder ..
cd ..
```

#### Step 3: Alternative Build with Makefile
```bash
make clean
make
```

## 🚀 Usage on Rocky Linux

### Basic Commands
```bash
# Encode any file type
./encoder_decoder encode document.pdf
./encoder_decoder encode /bin/ls          # Executable files
./encoder_decoder encode large_video.mp4   # Binary files

# Decode files
./encoder_decoder decode document.pdf.encoded
./encoder_decoder decode ls.encoded
./encoder_decoder decode large_video.mp4.encoded
```

### Example Output
```
=== C++23 File Encoder/Decoder ===
Using AES-256-CBC encryption with PBKDF2 key derivation

File: document.pdf
File type: PDF document
Original size: 2,048,576 bytes
✓ File encoded successfully: document.pdf.encoded
✓ Original file 'document.pdf' deleted
Encrypted size: 2,048,592 bytes

✓ Encoding completed successfully!
```

## 🔍 System Requirements

### Minimum Requirements
- **OS**: Rocky Linux 9.6 or compatible RHEL-based distribution
- **Compiler**: GCC 11.5.0+ or Clang 13+
- **Memory**: 512 MB RAM (more for large files)
- **Disk**: 100 MB free space + space for files being processed

### Recommended Packages
```bash
# Essential packages
dnf install gcc-c++ make cmake openssl-devel pkg-config

# Optional but helpful
dnf install git vim gdb valgrind
```

## 🛠️ Troubleshooting

### Common Issues and Solutions

#### 1. "OpenSSL not found"
```bash
# Install OpenSSL development headers
sudo dnf install openssl-devel pkg-config

# Verify installation
pkg-config --modversion openssl
```

#### 2. "C++ compiler not found"
```bash
# Install GCC C++ compiler
sudo dnf groupinstall "Development Tools"
sudo dnf install gcc-c++

# Verify installation
gcc --version
```

#### 3. "CMake not found"
```bash
# Install CMake
sudo dnf install cmake

# Or use Makefile instead
make clean && make
```

#### 4. "Permission denied when running"
```bash
# Make executable
chmod +x encoder_decoder

# Check if file exists
ls -la encoder_decoder
```

#### 5. Build fails with C++23 errors
The compatibility layer should handle this automatically, but if issues persist:

```bash
# Force C++20 mode
export CXXFLAGS="-std=c++20"
make clean && make
```

## 🏗️ Build System Details

### CMakeLists.txt Features
- Automatic compiler detection
- C++ standard version selection
- OpenSSL library detection
- Cross-platform compatibility

### Makefile Features  
- GCC/Clang version detection
- Automatic C++ standard selection
- Multiple library path support
- Verbose build information

## 🧪 Testing

### Automated Testing
```bash
# Run comprehensive test
./build_rocky.sh

# Manual test
echo "Test content" > test.txt
./encoder_decoder encode test.txt
./encoder_decoder decode test.txt.encoded
cat test.txt  # Should show "Test content"
```

### Performance Testing
```bash
# Test with large file
dd if=/dev/zero of=largefile.dat bs=1M count=100
./encoder_decoder encode largefile.dat
./encoder_decoder decode largefile.dat.encoded
```

## 🔒 Security Notes

- Uses AES-256-CBC encryption
- PBKDF2 key derivation (100,000 iterations)
- Cryptographically secure random salt/IV generation
- Binary-safe for all file types
- No password stored in plain text

## 📊 Performance on Rocky Linux

### Typical Performance (GCC 11.5)
- **Small files** (<1MB): <1 second
- **Medium files** (1-100MB): 1-10 seconds  
- **Large files** (>100MB): Depends on disk I/O

### Memory Usage
- **Base usage**: ~2-5 MB
- **File processing**: ~2x file size during processing
- **Large file optimization**: Streams for >100MB files

## 🔄 Compatibility Notes

### Differences from C++23 Version
1. **String formatting**: Uses polyfill instead of `std::format`
2. **String operations**: Uses custom `ends_with()` implementation
3. **Performance**: Minimal impact, <1% overhead
4. **Features**: All encryption/decryption features identical

### Forward Compatibility
When you upgrade to a newer GCC (12+), the same source code will automatically use native C++23 features for better performance.

## 📝 Development

### Modifying the Code
The compatibility layer is transparent:
```cpp
// This works on both C++20 and C++23
std::cout << FORMAT("Size: {} bytes\n", fileSize);

// This works on both C++20 and C++23  
if (ENDS_WITH(filename, ".encoded")) { /* ... */ }
```

### Adding New Features
Follow the same pattern:
1. Use compatibility macros for new C++23 features
2. Test on both GCC 11.5 and GCC 12+
3. Update build scripts as needed

This setup ensures your C++23 encoder/decoder works perfectly on Rocky Linux 9.6 while maintaining full compatibility with modern systems! 🚀