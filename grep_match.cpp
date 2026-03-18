#include "grep_match.h"
#include <algorithm>

std::vector<size_t> computeLPS(const std::string& pat) {
    size_t M = pat.size();
    std::vector<size_t> lps(M, 0);
    size_t len = 0;
    size_t i = 1;

    while (i < M) {
        if (pat[i] == pat[len]) {
            len++;
            lps[i] = len;
            i++;
        } else {
            if (len != 0) {
                len = lps[len - 1];
            } else {
                lps[i] = 0;
                i++;
            }
        }
    }
    return lps;
}

bool kmpSearch(const std::string& text, const std::string& pat, bool case_insensitive) {
    std::string txt = text;
    std::string pattern = pat;

    if (case_insensitive) {
        std::transform(txt.begin(), txt.end(), txt.begin(), ::tolower);
        std::transform(pattern.begin(), pattern.end(), pattern.begin(), ::tolower);
    }

    size_t N = txt.size();
    size_t M = pattern.size();

    if (M == 0) return true;
    if (N < M) return false;

    std::vector<size_t> lps = computeLPS(pattern);

    size_t i = 0; // index for txt[]
    size_t j = 0; // index for pattern[]

    while (i < N) {
        if (pattern[j] == txt[i]) {
            j++;
            i++;
        }

        if (j == M) {
            return true;
        } else if (i < N && pattern[j] != txt[i]) {
            if (j != 0) {
                j = lps[j - 1];
            } else {
                i++;
            }
        }
    }
    return false;
}
