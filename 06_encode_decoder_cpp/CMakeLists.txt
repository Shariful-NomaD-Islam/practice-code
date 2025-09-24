cmake_minimum_required(VERSION 3.20)
project(FileEncoderDecoder)

# Set C++23 standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add executable
add_executable(encoder_decoder main.cpp)

# Add local include directory
target_include_directories(encoder_decoder PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Link OpenSSL libraries (using system libraries)
target_link_libraries(encoder_decoder ssl crypto)

# Set compiler flags for better optimization and warnings
target_compile_options(encoder_decoder PRIVATE
    -Wall -Wextra -Wpedantic
    -O2
)

# Print build information
message(STATUS "Using local OpenSSL headers from: ${CMAKE_CURRENT_SOURCE_DIR}/include")
message(STATUS "Linking against system OpenSSL libraries")