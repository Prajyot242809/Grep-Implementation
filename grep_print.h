#infndef GREP_PRINT_H
#define GREP_PRINT_H

#include <string>
#include "grep_flags.h"

void print_grep_match
(
 const std::string& filename,
 int line_number,
 const std::string& line,
 const std::string& pattern,
 const GrepFlag& flags,
 bool multiple_files
);

#endif
