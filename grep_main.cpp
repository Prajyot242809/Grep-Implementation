#include <iostream>
#include <string>
#include <fstream>

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage:" << argv[0] << "PATTERN [file1 file2 ...]\n";
        std::cerr << "(or pipe input: cat file.txt |" << argv[0] << "PATTERN)\n";
        return 1;

    }

    std::string pattern = argv[1];

    //If no files are given as input
    if(argc == 2)
    {
        std::string line;
        while(std::getline(std::cin, line))
        {
            if (line.find(pattern) != std::string::npos)
            {
                std::cout << line << '\n';

            }
        }
        return 0;
    }
    //One or more files are given 
    bool multiple_files = (argc > 3);

    for (int i = 2; i < argc; ++i)
    {
        std::string filename = argv[i];
        std::ifstream file(filename);

        if(!file.is_open())
        {
            std::cerr << argv[0] << ":" << filename << ": No such file or directory\n";
            continue;
        }

        std::string line;
        int line_number = 0;

        while(std::getline(file, line))
        {
            ++line_number;
            if(line.find(pattern) != std::string::npos)
            {
                if(multiple_files)
                {
                    std::cout << filename << ":";
                }
            }
        }
        file.close();

    }
    return 0;
}
