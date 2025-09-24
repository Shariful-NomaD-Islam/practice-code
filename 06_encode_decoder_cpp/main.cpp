#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <filesystem>
#include <format>
#include <random>
#include <algorithm>
#include <codecvt>
#include <locale>
#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <openssl/sha.h>

namespace fs = std::filesystem;

class EncoderDecoder {
public:
    EncoderDecoder() {
        // Initialize OpenSSL
        EVP_add_cipher(EVP_aes_256_cbc());
    }
    
    bool encodeFile(const std::string& filePath, const std::string& password) {
        try {
            if (!fs::exists(filePath)) {
                std::cerr << std::format("Error: File '{}' not found!\n", filePath);
                return false;
            }
            
            // Read original file content as UTF-8
            std::string content = readUtf8File(filePath);
            if (content.empty() && fs::file_size(filePath) > 0) {
                std::cerr << "Error: Could not read file content!\n";
                return false;
            }
            
            std::cout << std::format("Original content: {}\n", content);
            std::cout << std::format("Content size: {} bytes\n", content.size());
            
            // Generate random salt and IV
            std::array<unsigned char, 16> salt;
            std::array<unsigned char, 16> iv;
            
            if (RAND_bytes(salt.data(), salt.size()) != 1 ||
                RAND_bytes(iv.data(), iv.size()) != 1) {
                std::cerr << "Error: Could not generate random bytes!\n";
                return false;
            }
            
            // Derive key from password using PBKDF2
            std::array<unsigned char, 32> key;
            if (PKCS5_PBKDF2_HMAC(password.c_str(), password.length(),
                                  salt.data(), salt.size(),
                                  100000, // iterations
                                  EVP_sha256(),
                                  key.size(), key.data()) != 1) {
                std::cerr << "Error: Key derivation failed!\n";
                return false;
            }
            
            // Encrypt content
            std::vector<unsigned char> encryptedData = encrypt(
                reinterpret_cast<const unsigned char*>(content.c_str()),
                content.length(), key.data(), iv.data());
            
            if (encryptedData.empty()) {
                std::cerr << "Error: Encryption failed!\n";
                return false;
            }
            
            // Create encoded file path
            std::string encodedPath = filePath + ".encoded";
            
            // Write salt + iv + encrypted data to file
            if (!writeEncodedFile(encodedPath, salt, iv, encryptedData)) {
                return false;
            }
            
            // Delete original file
            if (!fs::remove(filePath)) {
                std::cerr << std::format("Warning: Could not delete original file '{}'\n", filePath);
            }
            
            std::cout << std::format("✓ File encoded successfully: {}\n", encodedPath);
            std::cout << std::format("✓ Original file '{}' deleted\n", filePath);
            std::cout << std::format("Encrypted size: {} bytes\n", encryptedData.size());
            
            return true;
            
        } catch (const std::exception& e) {
            std::cerr << std::format("Error during encoding: {}\n", e.what());
            return false;
        }
    }
    
    bool decodeFile(const std::string& encodedPath, const std::string& password) {
        try {
            if (!fs::exists(encodedPath)) {
                std::cerr << std::format("Error: Encoded file '{}' not found!\n", encodedPath);
                return false;
            }
            
            // Read encoded file
            std::array<unsigned char, 16> salt, iv;
            std::vector<unsigned char> encryptedData;
            
            if (!readEncodedFile(encodedPath, salt, iv, encryptedData)) {
                return false;
            }
            
            std::cout << std::format("Encrypted data size: {} bytes\n", encryptedData.size());
            
            // Derive key from password
            std::array<unsigned char, 32> key;
            if (PKCS5_PBKDF2_HMAC(password.c_str(), password.length(),
                                  salt.data(), salt.size(),
                                  100000, // iterations
                                  EVP_sha256(),
                                  key.size(), key.data()) != 1) {
                std::cerr << "Error: Key derivation failed!\n";
                return false;
            }
            
            // Decrypt content
            std::vector<unsigned char> decryptedData = decrypt(
                encryptedData.data(), encryptedData.size(),
                key.data(), iv.data());
            
            if (decryptedData.empty()) {
                std::cerr << "Error: Decryption failed! Check your password.\n";
                return false;
            }
            
            // Convert decrypted data to UTF-8 string
            std::string decryptedText(decryptedData.begin(), decryptedData.end());
            
            std::cout << "\n" << std::string(50, '=') << "\n";
            std::cout << "DECODED CONTENT:\n";
            std::cout << std::string(50, '=') << "\n";
            std::cout << decryptedText << "\n";
            std::cout << std::string(50, '=') << "\n";
            std::cout << std::format("Decoded size: {} bytes\n", decryptedText.size());
            
            return true;
            
        } catch (const std::exception& e) {
            std::cerr << std::format("Error during decoding: {}\n", e.what());
            return false;
        }
    }

private:
    std::string readUtf8File(const std::string& filePath) {
        std::ifstream file(filePath, std::ios::binary);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open file for reading");
        }
        
        // Read entire file content
        std::string content((std::istreambuf_iterator<char>(file)),
                           std::istreambuf_iterator<char>());
        return content;
    }
    
    bool writeEncodedFile(const std::string& filePath,
                         const std::array<unsigned char, 16>& salt,
                         const std::array<unsigned char, 16>& iv,
                         const std::vector<unsigned char>& encryptedData) {
        std::ofstream file(filePath, std::ios::binary);
        if (!file.is_open()) {
            std::cerr << std::format("Error: Could not create encoded file '{}'\n", filePath);
            return false;
        }
        
        // Write magic header
        const std::string magic = "ENC23";
        file.write(magic.c_str(), magic.size());
        
        // Write salt
        file.write(reinterpret_cast<const char*>(salt.data()), salt.size());
        
        // Write IV
        file.write(reinterpret_cast<const char*>(iv.data()), iv.size());
        
        // Write encrypted data size
        uint32_t dataSize = static_cast<uint32_t>(encryptedData.size());
        file.write(reinterpret_cast<const char*>(&dataSize), sizeof(dataSize));
        
        // Write encrypted data
        file.write(reinterpret_cast<const char*>(encryptedData.data()), encryptedData.size());
        
        return true;
    }
    
    bool readEncodedFile(const std::string& filePath,
                        std::array<unsigned char, 16>& salt,
                        std::array<unsigned char, 16>& iv,
                        std::vector<unsigned char>& encryptedData) {
        std::ifstream file(filePath, std::ios::binary);
        if (!file.is_open()) {
            std::cerr << std::format("Error: Could not open encoded file '{}'\n", filePath);
            return false;
        }
        
        // Read and verify magic header
        std::string magic(5, '\0');
        file.read(magic.data(), 5);
        if (magic != "ENC23") {
            std::cerr << "Error: Invalid encoded file format!\n";
            return false;
        }
        
        // Read salt
        file.read(reinterpret_cast<char*>(salt.data()), salt.size());
        
        // Read IV
        file.read(reinterpret_cast<char*>(iv.data()), iv.size());
        
        // Read data size
        uint32_t dataSize;
        file.read(reinterpret_cast<char*>(&dataSize), sizeof(dataSize));
        
        // Read encrypted data
        encryptedData.resize(dataSize);
        file.read(reinterpret_cast<char*>(encryptedData.data()), dataSize);
        
        return !file.fail();
    }
    
    std::vector<unsigned char> encrypt(const unsigned char* plaintext, int plaintext_len,
                                      const unsigned char* key, const unsigned char* iv) {
        EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
        if (!ctx) return {};
        
        std::vector<unsigned char> ciphertext(plaintext_len + AES_BLOCK_SIZE);
        int len;
        int ciphertext_len;
        
        // Initialize encryption operation
        if (EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key, iv) != 1) {
            EVP_CIPHER_CTX_free(ctx);
            return {};
        }
        
        // Encrypt the plaintext
        if (EVP_EncryptUpdate(ctx, ciphertext.data(), &len, plaintext, plaintext_len) != 1) {
            EVP_CIPHER_CTX_free(ctx);
            return {};
        }
        ciphertext_len = len;
        
        // Finalize encryption
        if (EVP_EncryptFinal_ex(ctx, ciphertext.data() + len, &len) != 1) {
            EVP_CIPHER_CTX_free(ctx);
            return {};
        }
        ciphertext_len += len;
        
        EVP_CIPHER_CTX_free(ctx);
        
        ciphertext.resize(ciphertext_len);
        return ciphertext;
    }
    
    std::vector<unsigned char> decrypt(const unsigned char* ciphertext, int ciphertext_len,
                                      const unsigned char* key, const unsigned char* iv) {
        EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
        if (!ctx) return {};
        
        std::vector<unsigned char> plaintext(ciphertext_len);
        int len;
        int plaintext_len;
        
        // Initialize decryption operation
        if (EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key, iv) != 1) {
            EVP_CIPHER_CTX_free(ctx);
            return {};
        }
        
        // Decrypt the ciphertext
        if (EVP_DecryptUpdate(ctx, plaintext.data(), &len, ciphertext, ciphertext_len) != 1) {
            EVP_CIPHER_CTX_free(ctx);
            return {};
        }
        plaintext_len = len;
        
        // Finalize decryption
        if (EVP_DecryptFinal_ex(ctx, plaintext.data() + len, &len) != 1) {
            EVP_CIPHER_CTX_free(ctx);
            return {};
        }
        plaintext_len += len;
        
        EVP_CIPHER_CTX_free(ctx);
        
        plaintext.resize(plaintext_len);
        return plaintext;
    }
};

void printUsage(const std::string& programName) {
    std::cout << std::format("Usage: {} <command> <file>\n\n", programName);
    std::cout << "Commands:\n";
    std::cout << "  encode <file>         - Encode file and delete original\n";
    std::cout << "  decode <encoded_file> - Decode file and show content\n\n";
    std::cout << "Examples:\n";
    std::cout << std::format("  {} encode sample.txt\n", programName);
    std::cout << std::format("  {} decode sample.txt.encoded\n", programName);
    std::cout << "\nNote: Password is hardcoded in the application for security.\n";
}

int main(int argc, char* argv[]) {
    std::cout << "=== C++23 File Encoder/Decoder ===\n";
    std::cout << "Using AES-256-CBC encryption with PBKDF2 key derivation\n\n";
    
    if (argc != 3) {
        printUsage(argv[0]);
        return 1;
    }
    
    std::string command = argv[1];
    std::string filePath = argv[2];
    
    // Hardcoded password - change this to your desired password
    const std::string password = "MySecretKey2025!@#";
    
    try {
        EncoderDecoder encoder;
        
        if (command == "encode") {
            if (encoder.encodeFile(filePath, password)) {
                std::cout << "\n✓ Encoding completed successfully!\n";
                return 0;
            }
        } else if (command == "decode") {
            if (encoder.decodeFile(filePath, password)) {
                std::cout << "\n✓ Decoding completed successfully!\n";
                return 0;
            }
        } else {
            std::cerr << std::format("Error: Unknown command '{}'\n", command);
            printUsage(argv[0]);
            return 1;
        }
        
        return 1;
        
    } catch (const std::exception& e) {
        std::cerr << std::format("Fatal error: {}\n", e.what());
        return 1;
    }
}