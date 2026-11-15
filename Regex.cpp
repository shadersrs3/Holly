#include "Regex.h"

void ConstantStringExecutor::setConstStringRule(const std::string& str) {
    conststring = str;
}

int ConstantStringExecutor::execute(const std::string& str) {
    int i;
    bool matched = true;
    for (i = 0; i < conststring.length() && i < str.length(); i++) {
        if (conststring[i] != str[i]) {
            matched = false;
            break;
        }
    }
    return i == conststring.length() && matched ? (int)conststring.length() : 0;
}

void RangeStringExecutor::addRange(const std::string& range) {
    this->range.push_back(range);
}

void RangeStringExecutor::setLoop(bool state) {
    loop = state;
}

int RangeStringExecutor::execute(const std::string& str) {
    int r = 0;
    for (auto& k : range) {
        for (auto& i : range) {
            bool valid = false;
            for (int j = r; j < str.length(); j++) {
                if (str[j] >= i[0] && str[j] <= i[2] || str[j] >= k[0] && str[j] <= k[2]) {
                    if (!loop)
                        return 1;

                    r++;
                } else
                    break;
            }
        }
    }
    return r;
}