#include <iostream>
#include <string>
#include <fstream>

#include "grep_flags.h"
#include "grep_print.h"

int main(int argc, char* argv[])
{
    if (argc < 2) 
    {
        std::cerr << "Usage: " << argv[0] << " PATTERN [-n] [--color] [file1 file2 ...]\n";
        std::cerr << "  -n       \tshow line numbers\n";
        std::cerr << "  --color  \thighlight matches in red\n";
        std::cerr << "Examples:\n";
        std::cerr << "  " << argv[0] << " hello file.txt\n";
        std::cerr << "  echo \"hello world\" | " << argv[0] << " hello -n --color\n";
        return 1;
    }

    GrepFlags flags;
    std::string pattern = argv[1];
    int file_start = 2;

    // Parse flags (simple loop - stops when non-flag argument is found)
    while (file_start < argc) 
    {
        std::string arg = argv[file_start];
        if (arg == "-n") 
        {
            flags.show_line_numbers = true;
            file_start++;
        }
        else if (arg == "--color") 
        {
            flags.color_highlight = true;
            file_start++;
        }
        else 
        {
            break;  // this should be the first filename (or end of args)
        }
    }

    bool multiple_files = (argc - file_start > 1);

    // === Case 1: No files provided → read from stdin ===
    if (file_start == argc) 
    {
        std::string line;
        int line_num = 0;

        while (std::getline(std::cin, line)) 
        {
            ++line_num;
            if (line.find(pattern) != std::string::npos) 
            {
                print_grep_match("", line_num, line, pattern, flags, false);
            }
        }
        return 0;
    }

    // === Case 2: One or more files provided ===
    for (int i = file_start; i < argc; ++i) 
    {
        std::string filename = argv[i];
        std::ifstream file(filename);

        if (!file.is_open()) 
        {
            std::cerr << argv[0] << ": " << filename << ": No such file or directory\n";
            continue;
        }

        std::string line;
        int line_num = 0;

        while (std::getline(file, line)) 
        {
            ++line_num;
            if (line.find(pattern) != std::string::npos) 
            {
                print_grep_match(filename, line_num, line, pattern, flags, multiple_files);
            }
        }

        file.close();  // good practice, though destructor would close it anyway
    }

    return 0;
}
