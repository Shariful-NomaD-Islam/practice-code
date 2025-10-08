cmake_minimum_required(VERSION 3.10)
project(example VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source files
set(SOURCE_FILES
    example.cpp
)

# Create static library
add_library(example STATIC ${SOURCE_FILES})

# Set output directory for the library
set_target_properties(example PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/../lib"
)

# Include directories (for header files)
target_include_directories(example PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Installation rules (optional)
install(TARGETS example
    ARCHIVE DESTINATION lib
)

install(FILES example.h
    DESTINATION include
)
