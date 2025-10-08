use std::os::raw::c_char;

// Raw FFI declarations - kept private
#[link(name = "example", kind = "static")]
extern "C" {
    pub(crate) fn add(a: i32, b: i32) -> i32;
    pub(crate) fn multiply(a: i32, b: i32) -> i32;
    pub(crate) fn print_hello(name: *const c_char);
}
