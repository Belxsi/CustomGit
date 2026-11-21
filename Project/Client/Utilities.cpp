#include "Utilities.h"
std::vector<std::string> Utilities::splitString(
    const std::string& s,
    const std::string& delimiter,
    char glueChar,
    bool removeGlueChar
) {
    std::vector<std::string> tokens;
    if (delimiter.empty()) {
        tokens.emplace_back(s);
    }
    else {
        bool inGlue = false;
        size_t start = 0;
        size_t i = 0;

        while (i < s.size()) {
            if (glueChar != '\0' && s[i] == glueChar) {
                inGlue = !inGlue;
                ++i;
                continue;
            }
            if (!inGlue && s.compare(i, delimiter.size(), delimiter) == 0) {
                tokens.emplace_back(s, start, i - start);
                i += delimiter.size();
                start = i;
            }
            else {
                ++i;
            }
        }
        tokens.emplace_back(s, start);
    }

    if (removeGlueChar && glueChar != '\0') {
        for (auto& t : tokens) {
            if (t.size() >= 2 && t.front() == glueChar && t.back() == glueChar) {
                t.erase(t.size() - 1, 1);
                t.erase(0, 1);
            }
        }
    }

    return tokens;
}