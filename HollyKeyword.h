#pragma once

#include <string>
#include <vector>

enum KeywordId {
    Keyword_Name,
    Keyword_Octal,
    Keyword_Integer,
    Keyword_Hexadecimal,
    Keyword_FloatingPoint,
    Keyword_Multicharacter,
    Keyword_Singlecharacter,

    Custom_KeywordStart = 100,
    Custom_KeywordEnd = 10000
};

struct ConstKeyword {
    int id;
    std::string type;
    std::string keyword;
};

extern std::vector<ConstKeyword> __hollyEnumeratorNames;
int FindHollyKeyword(const char *enumeratorName);
void InitializeHollyKeyword(const std::vector<ConstKeyword>& keywords);