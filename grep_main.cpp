#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include "grep_flags.h"
#include "grep_print.h"


int main(int argc, char* argv[])
{
    
    if (argc < 2) 
    {
        std::cerr << "Usage: " << argv[0] << " PATTERN [flags] [file1 file2 ...]\n";
        std::cerr << "Flags you can use: -n, --color\n";
        std::cerr << "You can put flags anywhere after the pattern – super flexible!\n";
        std::cerr << "Quick examples:\n";
        std::cerr << "  " << argv[0] << " hello file.txt -n --color\n";
        std::cerr << "  " << argv[0] << " -n hello test.txt\n";
        std::cerr << "  echo \"hello world\" | " << argv[0] << " hello --color\n";
        return 1;
    }

    // Flags
    GrepFlags flags;

    std::string pattern = argv[1];
    std::vector<std::string> files;

    for (int i = 2; i < argc; ++i) 
    {
        std::string arg = argv[i];

        if (arg == "-n") 
        {
            flags.show_line_numbers = true;
        }
        else if (arg == "--color") 
        {
            flags.color_highlight = true;
        }
        else 
        {
            files.push_back(arg);
        }
    }

    bool multiple_files = (files.size() > 1);

    // No files given for pattern matching
    if (files.empty()) 
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

    for (const auto& filename : files) 
    {
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
    }

    return 0;
}
