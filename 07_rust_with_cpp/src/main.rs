mod ffi;
mod wrapper;

use wrapper::{add_numbers, multiply_numbers, print_greeting};

fn main() {
    let sum = add_numbers(10, 15);
    println!("10 + 15 = {}", sum);

    let product = multiply_numbers(6, 9);
    println!("6 * 9 = {}", product);

    match print_greeting("Rustacean") {
        Ok(_) => println!("Greeting printed successfully"),
        Err(e) => eprintln!("Error: {}", e),
    }
}
