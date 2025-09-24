# Local OpenSSL Headers

This directory contains a local copy of the OpenSSL headers needed for the C++23 File Encoder/Decoder project.

## Purpose

These headers are included to:
1. **Eliminate dependency issues**: No need to configure system OpenSSL paths
2. **Fix VS Code IntelliSense**: Headers are now in a known, local location
3. **Improve portability**: Project is more self-contained
4. **Consistent development**: All developers will use the same header versions

## Contents

The `openssl/` subdirectory contains all OpenSSL 3.5.1 header files copied from:
```
/opt/homebrew/Cellar/openssl@3/3.5.1/include/openssl/
```

## Key Headers Used

- `evp.h` - High-level cryptographic functions (EVP API)
- `aes.h` - AES encryption constants and definitions  
- `rand.h` - Random number generation
- `sha.h` - SHA hashing functions

## Build Configuration

The project build files are configured to use these local headers:

### Makefile
```makefile
CXXFLAGS = -std=c++23 -Wall -Wextra -Wpedantic -O2 -I./include
```

### CMakeLists.txt
```cmake
target_include_directories(encoder_decoder PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
```

### VS Code (c_cpp_properties.json)
```json
"includePath": [
    "${workspaceFolder}/include"
]
```

## Library Linking

While headers are local, the project still links against system OpenSSL libraries:
- `-lssl` - OpenSSL SSL/TLS library
- `-lcrypto` - OpenSSL cryptography library

## Version Information

- OpenSSL Version: 3.5.1
- Source: Homebrew installation on macOS
- Copied on: September 24, 2025

## Maintenance

If OpenSSL is updated, these headers may need to be refreshed:
```bash
cp -r /opt/homebrew/Cellar/openssl@3/[version]/include/openssl/* ./include/openssl/
```