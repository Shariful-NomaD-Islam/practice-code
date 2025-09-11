// Write a function in C++23 to find the longest substring without repeating characters.
#include <string>
#include <unordered_map>
#include <algorithm>
#include <iostream>
#include <vector>

template <typename T>
concept StringLike = requires(T s, typename T::value_type c) {
    { s.size() } -> std::convertible_to<std::size_t>;
    { s.begin() } -> std::input_iterator;
    { s.end() } -> std::input_iterator;
    { s[c] } -> std::convertible_to<typename T::value_type>;
    typename T::value_type;
};

std::string longest_substring_old(const std::string &s)
{
    if (s.empty())
        return "";

    std::unordered_map<char, size_t> char_index;
    size_t start = 0;
    size_t max_length = 0;
    size_t best_start = 0;

    for (size_t i = 0; i < s.length(); ++i)
    {
        if (char_index.contains(s[i]) && char_index[s[i]] >= start)
        {
            start = char_index[s[i]] + 1;
        }
        char_index[s[i]] = i;

        size_t current_length = i - start + 1;
        if (current_length > max_length)
        {
            max_length = current_length;
            best_start = start;
        }
    }

    return s.substr(best_start, max_length);
}

template <StringLike S>
std::basic_string<typename S::value_type> longest_substring(const S &s)
{
    using CharT = typename S::value_type;
    if (s.empty())
        return {};

    std::unordered_map<CharT, std::size_t> char_index;
    std::size_t start = 0, max_length = 0, best_start = 0;

    for (std::size_t i = 0; i < s.size(); ++i)
    {
        if (char_index.contains(s[i]) && char_index[s[i]] >= start)
        {
            start = char_index[s[i]] + 1;
        }
        char_index[s[i]] = i;

        std::size_t current_length = i - start + 1;
        if (current_length > max_length)
        {
            max_length = current_length;
            best_start = start;
        }
    }

    return std::basic_string<CharT>(s.begin() + best_start, s.begin() + best_start + max_length);
}

int main()
{
    std::string input = "abcabcbb";
    std::string result = longest_substring(input);
    std::cout << "Input: " << input << "\n";
    std::cout << "Longest unique substring: \"" << result << "\" (length: " << result.length() << ")\n";

    // Test more cases
    std::vector<std::string> test_cases = {"bbbbb", "pwwkew", "", "abcdef", "a"};
    for (const auto &test : test_cases)
    {
        auto res = longest_substring(test);
        std::cout << "Input: \"" << test << "\" -> \"" << res << "\" (length: " << res.length() << ")\n";
    }

    return 0;
}