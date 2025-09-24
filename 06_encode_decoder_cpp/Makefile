# Makefile for C++23 File Encoder/Decoder

# Compiler settings with version detection
CXX = g++

# Detect compiler version and set appropriate C++ standard
GCC_VERSION := $(shell $(CXX) -dumpversion | cut -f1 -d.)
CLANG_VERSION := $(shell $(CXX) --version 2>/dev/null | grep -o 'clang version [0-9]*' | cut -d' ' -f3)

ifeq ($(shell $(CXX) --version | grep -c "clang"), 1)
    # Clang compiler
    ifneq ($(shell test $(CLANG_VERSION) -ge 14; echo $$?), 0)
        CPPSTD = -std=c++20
        $(info Using C++20 for Clang $(CLANG_VERSION))
    else
        CPPSTD = -std=c++23
        $(info Using C++23 for Clang $(CLANG_VERSION))
    endif
else
    # GCC compiler
    ifneq ($(shell test $(GCC_VERSION) -ge 12; echo $$?), 0)
        CPPSTD = -std=c++20
        $(info Using C++20 for GCC $(GCC_VERSION))
    else
        CPPSTD = -std=c++23
        $(info Using C++23 for GCC $(GCC_VERSION))
    endif
endif

CXXFLAGS = $(CPPSTD) -Wall -Wextra -Wpedantic -O2 -I./include
OPENSSL_LIBPATH = -L/opt/homebrew/lib -L/usr/local/lib -L/usr/lib64 -L/usr/lib/x86_64-linux-gnu
OPENSSL_LIBS = -lssl -lcrypto

# Target executable
TARGET = encoder_decoder
SOURCE = main.cpp

# Default target
all: $(TARGET)

# Build the encoder/decoder
$(TARGET): $(SOURCE)
	$(CXX) $(CXXFLAGS) $(OPENSSL_LIBPATH) -o $(TARGET) $(SOURCE) $(OPENSSL_LIBS)

# Clean build files
clean:
	rm -f $(TARGET) *.encoded

# Install dependencies on macOS
install-deps:
	@echo "Installing OpenSSL via Homebrew..."
	brew install openssl pkg-config

# Test encoding
test-encode: $(TARGET)
	./$(TARGET) encode sample.txt

# Test decoding
test-decode: $(TARGET)
	@if [ -f sample.txt.encoded ]; then \
		./$(TARGET) decode sample.txt.encoded; \
	else \
		echo "No encoded file found. Run 'make test-encode' first."; \
	fi

# Complete test cycle
test: $(TARGET)
	@echo "Testing encode..."
	./$(TARGET) encode sample.txt
	@echo -e "\nTesting decode..."
	./$(TARGET) decode sample.txt.encoded

# Help target
help:
	@echo "Available targets:"
	@echo "  all          - Build the encoder/decoder"
	@echo "  clean        - Remove build files"
	@echo "  install-deps - Install OpenSSL dependencies (macOS)"
	@echo "  test-encode  - Test encoding sample.txt"
	@echo "  test-decode  - Test decoding sample.txt.encoded"
	@echo "  test         - Run complete encode/decode test"
	@echo "  help         - Show this help message"

.PHONY: all clean install-deps test-encode test-decode test help