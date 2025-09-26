cmake_minimum_required(VERSION 3.16)
project(FileEncoderDecoder)

# Detect compiler and set appropriate C++ standard
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "12.0")
        message(STATUS "GCC version ${CMAKE_CXX_COMPILER_VERSION} detected - using C++20 for compatibility")
        set(CMAKE_CXX_STANDARD 20)
    else()
        message(STATUS "GCC version ${CMAKE_CXX_COMPILER_VERSION} detected - using C++23")
        set(CMAKE_CXX_STANDARD 23)
    endif()
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "14.0")
        message(STATUS "Clang version ${CMAKE_CXX_COMPILER_VERSION} detected - using C++20 for compatibility")
        set(CMAKE_CXX_STANDARD 20)
    else()
        message(STATUS "Clang version ${CMAKE_CXX_COMPILER_VERSION} detected - using C++23")
        set(CMAKE_CXX_STANDARD 23)
    endif()
else()
    message(STATUS "Unknown compiler - attempting C++23")
    set(CMAKE_CXX_STANDARD 23)
endif()

set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add executable
add_executable(encoder_decoder main.cpp)

# Add local include directory (for our static OpenSSL headers)
target_include_directories(encoder_decoder PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Find and link OpenSSL libraries
if(APPLE)
    # macOS Homebrew OpenSSL paths
    set(OPENSSL_ROOT_DIR 
        "/opt/homebrew/opt/openssl@3"
        "/opt/homebrew/opt/openssl"
        "/usr/local/opt/openssl@3"
        "/usr/local/opt/openssl"
    )
    
    find_library(OPENSSL_SSL_LIBRARY 
        NAMES ssl libssl
        PATHS ${OPENSSL_ROOT_DIR}
        PATH_SUFFIXES lib
        REQUIRED
    )
    
    find_library(OPENSSL_CRYPTO_LIBRARY 
        NAMES crypto libcrypto
        PATHS ${OPENSSL_ROOT_DIR}
        PATH_SUFFIXES lib
        REQUIRED
    )
    
    target_link_libraries(encoder_decoder ${OPENSSL_SSL_LIBRARY} ${OPENSSL_CRYPTO_LIBRARY})
    message(STATUS "Found OpenSSL libraries:")
    message(STATUS "  SSL: ${OPENSSL_SSL_LIBRARY}")
    message(STATUS "  Crypto: ${OPENSSL_CRYPTO_LIBRARY}")
else()
    # Linux and other systems
    find_package(OpenSSL REQUIRED)
    target_link_libraries(encoder_decoder OpenSSL::SSL OpenSSL::Crypto)
endif()

# Set compiler flags for better optimization and warnings
target_compile_options(encoder_decoder PRIVATE
    -Wall -Wextra -Wpedantic
    -O2
)