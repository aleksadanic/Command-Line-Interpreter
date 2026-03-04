#include "Util.h"

int util::wordsCount(const std::string& str) {
    if (str.empty()) {
        return 0;
    }
    int ans = 0;
    for (int i = 1; i < (int)str.size(); i++) {
        if (!std::isspace((unsigned char)str[i - 1]) && std::isspace((unsigned char)str[i])) {
            ans++;
        }
    }
    if (!std::isspace((unsigned char)str.back())) {
        ans++;
    }
    return ans;
}

int util::charactersCount(const std::string& str) {
    return str.size();
}