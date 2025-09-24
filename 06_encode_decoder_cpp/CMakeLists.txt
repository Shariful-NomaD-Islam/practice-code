cmake_minimum_required(VERSION 3.20)
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