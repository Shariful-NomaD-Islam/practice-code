use std::ffi::CString;
use crate::ffi;

/// Safe wrapper for the add function
pub fn add_numbers(a: i32, b: i32) -> i32 {
    unsafe { ffi::add(a, b) }
}

/// Safe wrapper for the multiply function
pub fn multiply_numbers(a: i32, b: i32) -> i32 {
    unsafe { ffi::multiply(a, b) }
}

/// Safe wrapper for printing a greeting
pub fn print_greeting(name: &str) -> Result<(), std::ffi::NulError> {
    let c_name = CString::new(name)?;
    unsafe {
        ffi::print_hello(c_name.as_ptr());
    }
    Ok(())
}
