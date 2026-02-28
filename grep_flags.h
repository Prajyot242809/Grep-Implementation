#ifndef GREP_FLAGS_H
#define GREP_FLAGS_H

struct GrepFlags
{
    bool show_line_numbers = false;
    bool color_highlight   = false;
    bool case_insensitive  = false;
    bool invert_match      = false;
};

#endif
