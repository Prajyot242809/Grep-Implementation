#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

#include "grep_flags.h"
#include "grep_match.h"
#include "grep_print.h"

void process_file(const std::string& filename, const std::string& pattern, const GrepFlags& flags,
                  bool multiple_files, int& total_matches) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "grep: " << filename << ": No such file or directory\n";
        return;
    }

    std::string line;
    int line_num = 0;
    int count = 0;

    while (std::getline(file, line)) {
        ++line_num;
        bool matches = kmpSearch(line, pattern, flags.case_insensitive);
        if (flags.invert_match) matches = !matches;

        if (matches) {
            ++count;
            if (!flags.count_only && !flags.list_files_only) {
                print_match(filename, line_num, line, pattern, flags, multiple_files);
            }
        }
    }

    if (flags.count_only) {
        print_count(filename, count);              // ← fixed: removed , flags
    } else if (flags.list_files_only && count > 0) {
        print_filename_only(filename);
    }

    total_matches += count;
}

void process_directory(const std::string& dir_path, const std::string& pattern, const GrepFlags& flags,
                       bool multiple_files, int& total_matches) {
    DIR* dir = opendir(dir_path.c_str());
    if (!dir) return;

    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        if (std::string(entry->d_name) == "." || std::string(entry->d_name) == "..") continue;

        std::string path = dir_path + "/" + entry->d_name;
        struct stat st;
        if (stat(path.c_str(), &st) == 0) {
            if (S_ISDIR(st.st_mode)) {
                process_directory(path, pattern, flags, true, total_matches);
            } else if (S_ISREG(st.st_mode)) {
                process_file(path, pattern, flags, true, total_matches);
            }
        }
    }
    closedir(dir);
}

int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " PATTERN [flags] [path...]\n";
        std::cerr << "Flags: -n -i -v -c -l -r --color\n";
        return 2;
    }

    GrepFlags flags;
    std::string pattern = argv[1];
    std::vector<std::string> paths;

    for (int i = 2; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-n")      flags.show_line_numbers = true;
        else if (arg == "--color") flags.color_highlight = true;
        else if (arg == "-i") flags.case_insensitive = true;
        else if (arg == "-v") flags.invert_match = true;
        else if (arg == "-c") flags.count_only = true;
        else if (arg == "-l") flags.list_files_only = true;
        else if (arg == "-r" || arg == "-R") flags.recursive = true;
        else {
            paths.push_back(arg);
        }
    }

    bool multiple = (paths.size() > 1 || flags.recursive);
    int total_matches = 0;

    if (paths.empty()) {
        // stdin
        std::string line;
        int line_num = 0;
        int count = 0;

        while (std::getline(std::cin, line)) {
            ++line_num;
            bool matches = kmpSearch(line, pattern, flags.case_insensitive);
            if (flags.invert_match) matches = !matches;

            if (flags.count_only || flags.list_files_only) {
                if (matches) ++count;
            } else if (matches) {
                print_match("", line_num, line, pattern, flags, false);
            }
        }

        if (flags.count_only) print_count("", count);           // ← fixed: removed , flags
        if (flags.list_files_only && count > 0) print_filename_only("(stdin)");
        total_matches = count;
    } else {
        for (const auto& path : paths) {
            struct stat st;
            if (stat(path.c_str(), &st) == 0) {
                if (S_ISDIR(st.st_mode) && flags.recursive) {
                    process_directory(path, pattern, flags, multiple, total_matches);
                } else if (S_ISREG(st.st_mode)) {
                    process_file(path, pattern, flags, multiple, total_matches);
                }
            } else {
                std::cerr << argv[0] << ": " << path << ": No such file or directory\n";
            }
        }
    }

    // Exit codes like real grep
    if (total_matches > 0) return 0;
    if (total_matches == 0) return 1;
    return 2; // error occurred
}
