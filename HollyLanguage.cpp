#include "Lexer.h"
#include "HollyLanguage.h"

#include "HollyKeyword.h"

std::vector<ConstKeyword> __hollyEnumeratorNames = {
    { Keyword_Name , "Keyword_Name", "Keyword_Name", },
    { Keyword_Octal , "Keyword_Octal", "Keyword_Octal", },
    { Keyword_Integer , "Keyword_Integer", "Keyword_Integer", },
    { Keyword_Hexadecimal , "Keyword_Hexadecimal", "Keyword_Hexadecimal", },
    { Keyword_FloatingPoint , "Keyword_FloatingPoint", "Keyword_FloatingPoint", },
    { Keyword_Multicharacter , "Keyword_Multicharacter", "Keyword_Multicharacter", },
    { Keyword_Singlecharacter , "Keyword_Singlecharacter", "Keyword_Multicharacter", },
};

int FindHollyKeyword(const char *enumeratorName) {
    for (auto& i : __hollyEnumeratorNames)
        if (!strcmp(i.type.c_str(), enumeratorName))
            return i.id;
    return -1;
}

void InitializeLexerForHollyStartup(Lexer *lexer) {
    lexer->assignVariableToken("[a-zA-Z_-_][_a-zA-Z0-9_-_]*?", Keyword_Name, 0);
    lexer->assignVariableToken("[0-9][0-9]*?.[0-9]*?[a-zA-Z]*?", Keyword_FloatingPoint, 0);
    lexer->assignVariableToken("0x[0-9a-fA-F]*[a-zA-Z]*?", Keyword_Hexadecimal, 0);
    lexer->assignVariableToken("0[0-7]*[0-9a-zA-Z]*?", Keyword_Octal, 1);
    lexer->assignVariableToken("[0-9]*[a-zA-Z]*?", Keyword_Integer, 2);
    lexer->assignVariableToken("\"", Keyword_Multicharacter, -1); // -1 priority is a special case for string handling
    lexer->assignVariableToken("'", Keyword_Singlecharacter, -1); // -1 priority is a special case for string handling
    lexer->enableWhitespaceSkip();
}