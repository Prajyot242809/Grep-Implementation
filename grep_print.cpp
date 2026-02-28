#include "grep_print.h"
#include <algorithm>
#include <iostream>

//ANSI color codes to add
namespace ansi
{
    const std::string RESET = "\033[0m";
    const std::string CYAN   = "\033[1;36m";

}

void print_grep_match
(
 const std::string& filename,
 int line_number,
 const std::string& line,
 const std::string& pattern,
 const GrepFlags& flags,
 bool multiple_files
 )
{
    std::string text_to_check = line;
    std::string pat_to_check  = pattern;

    if (flags.case_insensitive)
    {
        std::transform(text_to_check.begin(), text_to_check.end(), text_to_check.begin(), ::tolower);
        std::transform(pat_to_check.begin(), pat_to_check.end(), pat_to_check.begin(), ::tolower);
    }
    bool matches = (text_to_check.find(pat_to_check) != std::string::npos);
    if (flags.invert_match)
    {
        matches = !matches;
    }
    if (!matches)
    {
        return;
    }

    std::string prefix;
    if(multiple_files)
    {
        prefix += filename + ":";
    }

    if(flags.show_line_numbers)
    {
        prefix += std::to_string(line_number) + ":";
    }

    if(!flags.color_highlight)
    {
        std::cout << prefix << line << '\n';
        return;
    }

    // Color version(highlight pattern occurrences)
    std::string colored = line;
    size_t pos = 0;

    while ((pos = colored.find(pattern, pos)) != std::string::npos)
    {
        colored.insert(pos, ansi::CYAN);
        pos += ansi::CYAN.length() + pattern.length();
        colored.insert(pos, ansi::RESET);
        pos += ansi::RESET.length();
    }

    std::cout << prefix << colored << '\n';
}
