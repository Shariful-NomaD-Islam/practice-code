# Java FileDecoder

A Java wrapper for the C++23 File Encoder/Decoder that provides a convenient way to decode encrypted files.

## Overview

`FileDecoder.java` is a simple Java program that invokes the C++ `encoder_decoder` executable to decode encrypted files. It provides:

- **Easy command-line interface** for decoding files
- **Comprehensive error checking** and user feedback
- **File type detection** and information display
- **Process management** for invoking the C++ decoder
- **Cross-platform compatibility** (works wherever Java runs)

## Prerequisites

- **Java Development Kit (JDK)** 8 or higher
- **C++ encoder_decoder executable** in the same directory
- **Encoded files** created by the C++ encoder

## Building

### Option 1: Use the build script
```bash
./build_java.sh
```

### Option 2: Manual compilation
```bash
javac FileDecoder.java
```

## Usage

### Basic Usage
```bash
java FileDecoder <encoded_file>
```

### Examples
```bash
# Decode a text document
java FileDecoder document.txt.encoded

# Decode a binary file  
java FileDecoder binary_test.dat.encoded

# Decode an executable
java FileDecoder program.exe.encoded

# Decode an image
java FileDecoder photo.jpg.encoded
```

### Help Message
```bash
java FileDecoder
# Shows: Usage: java FileDecoder <encoded_file>
```

## Features

### 1. **Comprehensive File Type Detection**
The program automatically detects and displays file types:
- Text files (`.txt`, `.cpp`, `.java`, `.py`, etc.)
- Images (`.jpg`, `.png`, `.gif`, etc.)  
- Videos (`.mp4`, `.avi`, `.mov`, etc.)
- Audio (`.mp3`, `.wav`, `.flac`, etc.)
- Documents (`.pdf`, `.docx`, `.xlsx`, etc.)
- Archives (`.zip`, `.tar`, `.gz`, etc.)
- Executables (`.exe`, `.app`, etc.)
- Binary data files

### 2. **Error Handling**
- Validates that encoded file exists
- Checks for C++ decoder executable
- Monitors decoding process exit codes
- Verifies output file creation
- Provides clear error messages

### 3. **Detailed Output**
```
=== Java File Decoder ===
Invoking C++ decoder for: test_document.txt.encoded

C++ Decoder Output:
-------------------
Encrypted data size: 176 bytes
Decoded size: 164 bytes
File type: Text file
✓ File decoded and saved as: test_document.txt
-------------------
✓ SUCCESS: File decoded successfully!
✓ Original file restored: test_document.txt
✓ File size: 164 bytes
✓ File type: Text file
```

### 4. **Process Integration**
- Executes C++ decoder as subprocess
- Captures and displays all output
- Merges stdout and stderr for complete feedback
- Proper process lifecycle management

## Integration with C++ Encoder/Decoder

This Java program is designed to work seamlessly with the C++23 File Encoder/Decoder:

### Encoding (C++)
```bash
./encoder_decoder encode myfile.pdf
# Creates: myfile.pdf.encoded (original deleted)
```

### Decoding (Java)
```bash
java FileDecoder myfile.pdf.encoded  
# Restores: myfile.pdf
```

## File Structure
```
06_encode_decoder_cpp/
├── encoder_decoder          # C++ executable
├── FileDecoder.java        # Java source code
├── FileDecoder.class       # Compiled Java bytecode
├── build_java.sh          # Java build script
└── *.encoded              # Encrypted files
```

## Security Notes

- The Java program does **NOT** handle encryption/decryption directly
- All cryptographic operations are performed by the C++ program
- The hardcoded password is managed by the C++ executable
- Java simply provides a user-friendly interface layer

## Error Codes

- **0**: Success - file decoded successfully
- **1**: Error - missing arguments, file not found, or decoding failed

## Supported Platforms

- **Windows**: Requires `encoder_decoder.exe`
- **macOS**: Requires `encoder_decoder` binary
- **Linux**: Requires `encoder_decoder` binary

The Java program automatically adapts to the underlying platform where the C++ executable runs.

## Example Workflow

1. **Compile Java decoder**:
   ```bash
   javac FileDecoder.java
   ```

2. **Encode a file with C++**:
   ```bash
   ./encoder_decoder encode important_document.pdf
   ```

3. **Decode using Java**:
   ```bash
   java FileDecoder important_document.pdf.encoded
   ```

4. **Result**: Original `important_document.pdf` is restored!

## Advantages of Java Wrapper

1. **Platform Independence**: Java runs everywhere
2. **Enhanced User Experience**: Better error messages and feedback
3. **Process Management**: Handles subprocess execution properly
4. **File Validation**: Comprehensive input validation
5. **Type Detection**: Automatic file type identification
6. **Integration Ready**: Easy to integrate into larger Java applications