#ifndef GREP_PRINT_H
#define GREP_PRINT_H

#include <string>
#include "grep_flags.h"

void print_match(const std::string& filename, int line_number, const std::string& line,
                 const std::string& pattern, const GrepFlags& flags, bool multiple_files);

void print_count(const std::string& filename, int count);

void print_filename_only(const std::string& filename);

#endif
