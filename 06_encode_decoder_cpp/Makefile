# Makefile for C++23 File Encoder/Decoder

# Compiler settings
CXX = g++
CXXFLAGS = -std=c++23 -Wall -Wextra -Wpedantic -O2
OPENSSL_CFLAGS = $(shell pkg-config --cflags openssl)
OPENSSL_LIBS = $(shell pkg-config --libs openssl)

# Target executable
TARGET = encoder_decoder
SOURCE = main.cpp

# Default target
all: $(TARGET)

# Build the encoder/decoder
$(TARGET): $(SOURCE)
	$(CXX) $(CXXFLAGS) $(OPENSSL_CFLAGS) -o $(TARGET) $(SOURCE) $(OPENSSL_LIBS)

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