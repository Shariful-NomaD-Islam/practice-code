cmake_minimum_required(VERSION 3.20)
project(FileEncoderDecoder)

# Set C++23 standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find OpenSSL
find_package(OpenSSL REQUIRED)

# Add executable
add_executable(encoder_decoder main.cpp)

# Link OpenSSL libraries
target_link_libraries(encoder_decoder OpenSSL::SSL OpenSSL::Crypto)

# Set compiler flags for better optimization and warnings
target_compile_options(encoder_decoder PRIVATE
    -Wall -Wextra -Wpedantic
    -O2
)

# Print OpenSSL information
message(STATUS "OpenSSL version: ${OPENSSL_VERSION}")
message(STATUS "OpenSSL include dir: ${OPENSSL_INCLUDE_DIR}")
message(STATUS "OpenSSL libraries: ${OPENSSL_LIBRARIES}")