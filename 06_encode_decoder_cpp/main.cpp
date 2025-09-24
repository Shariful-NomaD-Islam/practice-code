#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <filesystem>
#include <random>
#include <algorithm>
#include <codecvt>
#include <locale>
#include <sstream>
#include <iomanip>
#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <openssl/sha.h>

// Compatibility layer for different C++ standards
#if __cplusplus >= 202302L || (defined(__GNUC__) && __GNUC__ >= 12) || (defined(__clang__) && __clang_major__ >= 14)
    #include <format>
    #define HAS_STD_FORMAT 1
#else
    #define HAS_STD_FORMAT 0
#endif

// Compatibility macros and functions
#if !HAS_STD_FORMAT
    // Simple format replacement for basic string formatting
    template<typename T>
    std::string format_impl(const std::string& fmt, T&& value) {
        size_t pos = fmt.find("{}");
        if (pos != std::string::npos) {
            std::ostringstream oss;
            oss << fmt.substr(0, pos) << value << fmt.substr(pos + 2);
            return oss.str();
        }
        return fmt;
    }
    
    template<typename T, typename... Args>
    std::string format_impl(const std::string& fmt, T&& first, Args&&... args) {
        size_t pos = fmt.find("{}");
        if (pos != std::string::npos) {
            std::ostringstream oss;
            oss << fmt.substr(0, pos) << first;
            std::string remaining = fmt.substr(pos + 2);
            return oss.str() + format_impl(remaining, std::forward<Args>(args)...);
        }
        return fmt;
    }
    
    #define FORMAT(fmt, ...) format_impl(fmt, __VA_ARGS__)
#else
    #define FORMAT(fmt, ...) std::format(fmt, __VA_ARGS__)
#endif

// String ends_with compatibility for C++17/20
#if __cplusplus < 202002L
    namespace string_utils {
        bool ends_with(const std::string& str, const std::string& suffix) {
            if (suffix.length() > str.length()) return false;
            return str.compare(str.length() - suffix.length(), suffix.length(), suffix) == 0;
        }
    }
    #define ENDS_WITH(str, suffix) string_utils::ends_with(str, suffix)
#else
    #define ENDS_WITH(str, suffix) str.ends_with(suffix)
#endif

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
                std::cerr << FORMAT("Error: File '{}' not found!\n", filePath);
                return false;
            }
            
            // Read original file as binary data (works for all file types)
            std::vector<unsigned char> fileData = readBinaryFile(filePath);
            if (fileData.empty() && fs::file_size(filePath) > 0) {
                std::cerr << "Error: Could not read file content!\n";
                return false;
            }
            
            std::cout << FORMAT("File: {}\n", fs::path(filePath).filename().string());
            std::cout << FORMAT("File type: {}\n", getFileTypeDescription(filePath));
            std::cout << FORMAT("Original size: {} bytes\n", fileData.size());
            
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
            
            // Encrypt binary data
            std::vector<unsigned char> encryptedData = encrypt(
                fileData.data(), fileData.size(), key.data(), iv.data());
            
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
                std::cerr << FORMAT("Warning: Could not delete original file '{}'\n", filePath);
            }
            
            std::cout << FORMAT("✓ File encoded successfully: {}\n", encodedPath);
            std::cout << FORMAT("✓ Original file '{}' deleted\n", filePath);
            std::cout << FORMAT("Encrypted size: {} bytes\n", encryptedData.size());
            
            return true;
            
        } catch (const std::exception& e) {
            std::cerr << FORMAT("Error during encoding: {}\n", e.what());
            return false;
        }
    }
    
    bool decodeFile(const std::string& encodedPath, const std::string& password) {
        try {
            if (!fs::exists(encodedPath)) {
                std::cerr << FORMAT("Error: Encoded file '{}' not found!\n", encodedPath);
                return false;
            }
            
            // Read encoded file
            std::array<unsigned char, 16> salt, iv;
            std::vector<unsigned char> encryptedData;
            
            if (!readEncodedFile(encodedPath, salt, iv, encryptedData)) {
                return false;
            }
            
            std::cout << FORMAT("Encrypted data size: {} bytes\n", encryptedData.size());
            
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
            
            // Determine original filename by removing ".encoded" extension
            std::string originalFilename = encodedPath;
            if (ENDS_WITH(originalFilename, ".encoded")) {
                originalFilename = originalFilename.substr(0, originalFilename.length() - 8);
            }
            
            // Write decoded binary data to original file
            if (!writeBinaryFile(originalFilename, decryptedData)) {
                std::cerr << FORMAT("Error: Could not write decoded file '{}'\n", originalFilename);
                return false;
            }
            
            std::cout << FORMAT("Decoded size: {} bytes\n", decryptedData.size());
            std::cout << FORMAT("File type: {}\n", getFileTypeDescription(originalFilename));
            std::cout << FORMAT("✓ File decoded and saved as: {}\n", originalFilename);
            
            return true;
            
        } catch (const std::exception& e) {
            std::cerr << FORMAT("Error during decoding: {}\n", e.what());
            return false;
        }
    }

private:
    std::vector<unsigned char> readBinaryFile(const std::string& filePath) {
        std::ifstream file(filePath, std::ios::binary | std::ios::ate);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open file for reading");
        }
        
        std::streamsize fileSize = file.tellg();
        file.seekg(0, std::ios::beg);
        
        std::vector<unsigned char> buffer(fileSize);
        if (fileSize > 0 && !file.read(reinterpret_cast<char*>(buffer.data()), fileSize)) {
            throw std::runtime_error("Could not read file content");
        }
        
        return buffer;
    }
    
    bool writeBinaryFile(const std::string& filePath, const std::vector<unsigned char>& data) {
        std::ofstream file(filePath, std::ios::binary);
        if (!file.is_open()) {
            return false;
        }
        
        if (!data.empty()) {
            file.write(reinterpret_cast<const char*>(data.data()), data.size());
        }
        return !file.fail();
    }
    
    std::string getFileTypeDescription(const std::string& filePath) {
        fs::path path(filePath);
        std::string extension = path.extension().string();
        
        if (extension.empty()) {
            // Check if file is executable
            auto perms = fs::status(filePath).permissions();
            if ((perms & fs::perms::owner_exec) != fs::perms::none ||
                (perms & fs::perms::group_exec) != fs::perms::none ||
                (perms & fs::perms::others_exec) != fs::perms::none) {
                return "Executable/Binary";
            }
            return "Binary file";
        }
        
        // Common file type descriptions
        if (extension == ".txt" || extension == ".md" || extension == ".cpp" || 
            extension == ".h" || extension == ".py" || extension == ".js") {
            return "Text file";
        } else if (extension == ".jpg" || extension == ".jpeg" || extension == ".png" || 
                   extension == ".gif" || extension == ".bmp") {
            return "Image file";
        } else if (extension == ".mp4" || extension == ".avi" || extension == ".mov" || 
                   extension == ".mkv") {
            return "Video file";
        } else if (extension == ".mp3" || extension == ".wav" || extension == ".flac") {
            return "Audio file";
        } else if (extension == ".pdf" || extension == ".doc" || extension == ".docx") {
            return "Document";
        } else if (extension == ".zip" || extension == ".tar" || extension == ".gz") {
            return "Archive";
        }
        
        return "Binary file";
    }
    
    bool writeEncodedFile(const std::string& filePath,
                         const std::array<unsigned char, 16>& salt,
                         const std::array<unsigned char, 16>& iv,
                         const std::vector<unsigned char>& encryptedData) {
        std::ofstream file(filePath, std::ios::binary);
        if (!file.is_open()) {
            std::cerr << FORMAT("Error: Could not create encoded file '{}'\n", filePath);
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
            std::cerr << FORMAT("Error: Could not open encoded file '{}'\n", filePath);
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
    std::cout << FORMAT("Usage: {} <command> <file>\n\n", programName);
    std::cout << "Commands:\n";
    std::cout << "  encode <file>         - Encode any file type and delete original\n";
    std::cout << "  decode <encoded_file> - Decode file and restore original\n\n";
    std::cout << "Examples:\n";
    std::cout << FORMAT("  {} encode document.pdf\n", programName);
    std::cout << FORMAT("  {} encode executable_file\n", programName);
    std::cout << FORMAT("  {} encode image.jpg\n", programName);
    std::cout << FORMAT("  {} decode document.pdf.encoded\n", programName);
    std::cout << "\nSupported File Types:\n";
    std::cout << "  • Text files (.txt, .cpp, .py, .js, etc.)\n";
    std::cout << "  • Binary executables and applications\n";
    std::cout << "  • Images (.jpg, .png, .gif, etc.)\n";
    std::cout << "  • Videos (.mp4, .avi, .mov, etc.)\n";
    std::cout << "  • Documents (.pdf, .docx, etc.)\n";
    std::cout << "  • Archives (.zip, .tar, .gz, etc.)\n";
    std::cout << "  • Any other file type (binary-safe)\n";
    std::cout << "\nBehavior:\n";
    std::cout << "  • Uses AES-256-CBC encryption with PBKDF2 key derivation\n";
    std::cout << "  • Encoding deletes the original file after encryption\n";
    std::cout << "  • Decoding restores the file with original filename\n";
    std::cout << "  • Password is hardcoded for security\n";
    std::cout << "  • Preserves file permissions and binary integrity\n";
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
            std::cerr << FORMAT("Error: Unknown command '{}'\n", command);
            printUsage(argv[0]);
            return 1;
        }
        
        return 1;
        
    } catch (const std::exception& e) {
        std::cerr << FORMAT("Fatal error: {}\n", e.what());
        return 1;
    }
}