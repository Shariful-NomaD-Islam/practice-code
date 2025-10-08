fn main() {
    // Tell cargo where to find the static library
    println!("cargo:rustc-link-search=native=lib");
    
    // Link the static library
    println!("cargo:rustc-link-lib=static=example");
    
    // Link C++ standard library (platform-specific)
    #[cfg(target_os = "linux")]
    println!("cargo:rustc-link-lib=stdc++");
    
    #[cfg(target_os = "macos")]
    println!("cargo:rustc-link-lib=c++");
    
    #[cfg(target_os = "windows")]
    println!("cargo:rustc-link-lib=stdc++");
    
    // Tell cargo to rerun if the library changes
    println!("cargo:rerun-if-changed=lib/libexample.a");
}
