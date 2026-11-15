#include "Lexer.h"
#include "HollyKeyword.h"

struct HollyTestUnit {
    std::string value;
    int id;
};

std::vector<HollyTestUnit> varunits = {
    { "_", Keyword_Name },
    { "_a0b", Keyword_Name },
    { "0.f", Keyword_FloatingPoint },
    { "0.3f", Keyword_FloatingPoint },
    { "0xDEADBEEF", Keyword_Hexadecimal },
    { "0xDEADBEEFuLL", Keyword_Hexadecimal },
    { "0777", Keyword_Octal },
    { "0777ULL", Keyword_Octal },
    { "1234", Keyword_Integer },
    { "1234uLL", Keyword_Integer },
    { "1234uLL", Keyword_Hexadecimal }, // Will fail (useless test)
    { "\"hello world[]\\n\"", Keyword_Multicharacter },
    { "'['", Keyword_Singlecharacter },
};

std::vector<HollyTestUnit> constunits = {
    { "datastruct", ConstKeyword_datastruct },
    { "alias", ConstKeyword_alias },
    { "uint8", ConstKeyword_uint8 },
    { "uint16", ConstKeyword_uint16 },
    { "uint32", ConstKeyword_uint32 },
    { "uint64", ConstKeyword_uint64 },
    { "int8", ConstKeyword_int8 },
    { "int16", ConstKeyword_int16 },
    { "int32", ConstKeyword_int32 },
    { "int64", ConstKeyword_int64 },
};

static bool __testHollyVariableTokens(Lexer *lexer, const std::vector<HollyTestUnit>& units) {
    int pass = 0;

    printf("Variable token unit test\n");
    for (auto& i : units) {
        if (lexer->validVariableToken(i.value).first == i.id) {
            printf("Passed ID (%d) with string %s\n", i.id, i.value.c_str());
            pass++;
        } else {
            printf("Failed ID (%d) with string %s\n", i.id, i.value.c_str());
        }
    }

    printf("%d out of %zd passed.\n", pass, units.size());
    return pass == (int)units.size();
}

static bool __testHollyConstantTokens(Lexer *lexer, const std::vector<HollyTestUnit>& units) {
    int pass = 0;

    printf("Constant token unit test\n");
    for (auto& i : units) {
        if (lexer->validConstantToken(i.value).first == i.id) {
            printf("Passed ID (%d) with string %s\n", i.id, i.value.c_str());
            pass++;
        } else {
            printf("Failed ID (%d) with string %s\n", i.id, i.value.c_str());
        }
    }

    printf("%d out of %zd passed.\n", pass, units.size());
    return pass == (int)units.size();
}