#!/bin/bash

# Java FileDecoder Build Script

echo "=== Java FileDecoder Build Script ==="
echo

# Check if Java is available
if ! command -v javac >/dev/null 2>&1; then
    echo "Error: Java compiler (javac) not found!"
    echo "Please install Java Development Kit (JDK)"
    exit 1
fi

if ! command -v java >/dev/null 2>&1; then
    echo "Error: Java runtime (java) not found!"
    echo "Please install Java Runtime Environment (JRE) or JDK"
    exit 1
fi

echo "✓ Java compiler found: $(javac -version 2>&1)"
echo "✓ Java runtime found: $(java -version 2>&1 | head -n1)"
echo

# Compile the Java program
echo "Compiling FileDecoder.java..."
if javac FileDecoder.java; then
    echo "✓ Compilation successful!"
    echo
    echo "Usage:"
    echo "  java FileDecoder <encoded_file>"
    echo
    echo "Example:"
    echo "  java FileDecoder document.pdf.encoded"
    echo "  java FileDecoder binary_file.dat.encoded"
    echo
else
    echo "✗ Compilation failed!"
    exit 1
fi