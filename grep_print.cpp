#include "grep_print.h"
#include <iostream>

//ANSI color codes to add
namespace ansi
{
    const std::string RESET = "\033[0m";
    const std::string RED   = "\033[31m";

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
    std::string prefix;
    if(multiple_files)
    {
        prefix += filename + ":";
    }

    if(flag.show_line_numbers)
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
        colored.insert(pos, ansi::RED);
        pos += ansi::RED.length() + patteren.length();
        colored.insert(pos, ansi::RESET);
        pos += ansi::RESET.length();
    }

    std::cout << prefix << colored << '\n';
}
