#include "grep_print.h"
#include <iostream>
#include <algorithm>

namespace ansi 
{
    const std::string RESET = "\033[0m";
    const std::string CYAN  = "\033[1;36m";
}

void print_count(const std::string& filename, int count) 
{
    if (!filename.empty()) std::cout << filename << ":";
    std::cout << count << '\n';
}

void print_filename_only(const std::string& filename) 
{
    std::cout << filename << '\n';
}

void print_match(const std::string& filename, int line_number, const std::string& line,
                 const std::string& pattern, const GrepFlags& flags, bool multiple_files) 
{

    std::string prefix;
    if (multiple_files) prefix += filename + ":";
    if (flags.show_line_numbers) prefix += std::to_string(line_number) + ":";

    if (!flags.color_highlight) 
    {
        std::cout << prefix << line << '\n';
        return;
    }

    std::string colored = line;
    std::string search_text = line;
    std::string search_pat = pattern;

    if (flags.case_insensitive) 
    {
        std::transform(search_text.begin(), search_text.end(), search_text.begin(), ::tolower);
        std::transform(search_pat.begin(), search_pat.end(), search_pat.begin(), ::tolower);
    }

    size_t pos = 0;
    while ((pos = search_text.find(search_pat, pos)) != std::string::npos) 
    {
        colored.insert(pos, ansi::CYAN);
        pos += ansi::CYAN.length() + search_pat.length();
        colored.insert(pos, ansi::RESET);
        pos += ansi::RESET.length();
    }

    std::cout << prefix << colored << '\n';
}
