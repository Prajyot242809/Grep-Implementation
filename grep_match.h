#ifndef GREP_MATCH_H
#define GREP_MATCH_H

#include <string>
#include <vector>

std::vector<size_t> computeLPS(const std::string& pat);

bool kmpSearch(const std::string& text, const std::string& pat, bool case_insensitive = false);

#endif
