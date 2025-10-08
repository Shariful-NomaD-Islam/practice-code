# Rust + C++ FFI Project

A template for integrating Rust with C++ static libraries using safe FFI wrappers.

## Prerequisites

- CMake (3.10+)
- C++ compiler (g++, clang++, or MSVC)
- Rust (1.70+)

## Quick Start

**Build C++ library:**
```bash
cd cpp
mkdir build && cd build
cmake .. && cmake --build .
cd ../..
```

**Build and run:**
```bash
cargo run
```

Or

```bash
cargo build --release
./target/release/rust_with_cpp
```

## Project Structure

```bash
my_rust_cpp_project/
├── Cargo.toml
├── build.rs
├── cpp/
│   ├── CMakeLists.txt      # Build configuration for the C++ library
│   ├── example.h           # Header file
│   └── example.cpp         # Implementation file
├── lib/
│   └── libexample.a        # Generated static library (after building)
└── src/
    ├── main.rs             # Pure safe Rust code
    ├── ffi.rs              # Raw unsafe FFI bindings (hidden)
    └── wrapper.rs          # Safe wrapper API
```

## Architecture

- **ffi.rs**: Contains all `unsafe` FFI declarations
- **wrapper.rs**: Provides safe Rust API
- **main.rs**: 100% safe application code

No `unsafe` blocks needed in your application!

## License

MIT
