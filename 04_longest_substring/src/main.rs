use std::collections::HashSet;

fn longest_unique_substring(s: &str) -> &str {
    if s.is_empty() {
        return "";
    }
    
    let mut seen = HashSet::new();
    let (mut start, mut end, mut max_start, mut max_len) = (0, 0, 0, 0);
    let chars: Vec<char> = s.chars().collect();

    while end < chars.len() {
        if !seen.contains(&chars[end]) {
            seen.insert(chars[end]);
            end += 1;
            if end - start > max_len {
                max_len = end - start;
                max_start = start;
            }
        } else {
            seen.remove(&chars[start]);
            start += 1;
        }
    }

    let start_byte = s.char_indices().nth(max_start).map(|(i, _)| i).unwrap_or(0);
    let end_byte = s.char_indices().nth(max_start + max_len).map(|(i, _)| i).unwrap_or(s.len());

    &s[start_byte..end_byte]
}

fn main() {
    let input = "abcabcbb";
    let result = longest_unique_substring(input);
    println!("Input: {}", input);
    println!("Longest unique substring: \"{}\" (length: {})", result, result.len());

    // Test more cases
    let test_cases = ["bbbbb", "pwwkew", "", "abcdef", "a", "a🌍bca"];
    for &test in &test_cases {
        let res = longest_unique_substring(test);
        println!("Input: \"{}\" -> \"{}\" (length: {})", test, res, res.len());
    }
}
