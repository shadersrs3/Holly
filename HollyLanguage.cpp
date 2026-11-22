#include "Lexer.h"
#include "HollyLanguage.h"

static ConstKeyword __reservedHollyKeywords[] = {
    { "datastruct", ConstKeyword_datastruct },
    { "sharedstruct", ConstKeyword_sharedstruct },
    { "alias", ConstKeyword_alias },
    { "uint8", ConstKeyword_uint8 },
    { "uint16", ConstKeyword_uint16 },
    { "uint32", ConstKeyword_uint32 },
    { "uint64", ConstKeyword_uint64 },
    { "int8", ConstKeyword_int8 },
    { "int16", ConstKeyword_int16 },
    { "int32", ConstKeyword_int32 },
    { "int64", ConstKeyword_int64 },
    { "void", ConstKeyword_void },
};

static ConstKeyword __reservedHollyOperators[] = {
    { "->", OperatorKeyword_pointerstructaccess, },
    { ".", OperatorKeyword_structaccess, },
    { "(", OperatorKeyword_exprbegin, },
    { ")", OperatorKeyword_exprend, },
    { "+", OperatorKeyword_plus, },
    { "-", OperatorKeyword_minus, },
    { "*", OperatorKeyword_star, },
    { "/", OperatorKeyword_divide, },
    { "%", OperatorKeyword_modulo, },
    { "&", OperatorKeyword_bitand, },
    { "^", OperatorKeyword_bitxor, },
    { "|", OperatorKeyword_bitor, },
    { "~", OperatorKeyword_bitcompl, },
    { ">>",OperatorKeyword_shr, },
    { "<<", OperatorKeyword_shl, },
    { "=", OperatorKeyword_assignequal, },
    { "[", OperatorKeyword_bracketstart, },
    { "]", OperatorKeyword_bracketend, },
    { "...", OperatorKeyword_ellipsis, },
    { "{", OperatorKeyword_statementstart, },
    { "}", OperatorKeyword_statementend, },
    { ">=", OperatorKeyword_ge, },
    { "<=", OperatorKeyword_le, },
    { ">", OperatorKeyword_gt, },
    { "<", OperatorKeyword_lt, },
    { "==", OperatorKeyword_eq, },
    { "!=", OperatorKeyword_ne, },
    { "&&", OperatorKeyword_and, },
    { "||", OperatorKeyword_or, },
    { "!", OperatorKeyword_not, },
    { ",", OperatorKeyword_comma, },
    { ";", OperatorKeyword_endstmt, },
};

ConstKeyword __hollyEnumeratorNames[] = {
    { "Keyword_Name", Keyword_Name },
    { "Keyword_Octal", Keyword_Octal },
    { "Keyword_Integer", Keyword_Integer },
    { "Keyword_Hexadecimal", Keyword_Hexadecimal },
    { "Keyword_FloatingPoint", Keyword_FloatingPoint },
    { "Keyword_Multicharacter", Keyword_Multicharacter },
    { "Keyword_Singlecharacter", Keyword_Singlecharacter },
    { "ConstKeyword_datastruct", ConstKeyword_datastruct },
    { "ConstKeyword_sharedstruct", ConstKeyword_sharedstruct },
    { "ConstKeyword_int8", ConstKeyword_int8 },
    { "ConstKeyword_int16", ConstKeyword_int16 },
    { "ConstKeyword_int32", ConstKeyword_int32 },
    { "ConstKeyword_int64", ConstKeyword_int64 },
    { "ConstKeyword_uint8", ConstKeyword_uint8 },
    { "ConstKeyword_uint16", ConstKeyword_uint16 },
    { "ConstKeyword_uint32", ConstKeyword_uint32 },
    { "ConstKeyword_uint64", ConstKeyword_uint64 },
    { "ConstKeyword_void", ConstKeyword_void },
    { "ConstKeyword_alias", ConstKeyword_alias },
    { "OperatorKeyword_pointerstructaccess", OperatorKeyword_pointerstructaccess },
    { "OperatorKeyword_structaccess", OperatorKeyword_structaccess },
    { "OperatorKeyword_exprbegin", OperatorKeyword_exprbegin },
    { "OperatorKeyword_exprend", OperatorKeyword_exprend },
    { "OperatorKeyword_plus", OperatorKeyword_plus },
    { "OperatorKeyword_minus", OperatorKeyword_minus },
    { "OperatorKeyword_star", OperatorKeyword_star },
    { "OperatorKeyword_divide", OperatorKeyword_divide },
    { "OperatorKeyword_modulo", OperatorKeyword_modulo },
    { "OperatorKeyword_bitand", OperatorKeyword_bitand },
    { "OperatorKeyword_bitxor", OperatorKeyword_bitxor },
    { "OperatorKeyword_bitor", OperatorKeyword_bitor },
    { "OperatorKeyword_bitcompl", OperatorKeyword_bitcompl },
    { "OperatorKeyword_shr", OperatorKeyword_shr },
    { "OperatorKeyword_shl", OperatorKeyword_shl },
    { "OperatorKeyword_assignequal", OperatorKeyword_assignequal },
    { "OperatorKeyword_bracketstart", OperatorKeyword_bracketstart },
    { "OperatorKeyword_bracketend", OperatorKeyword_bracketend },
    { "OperatorKeyword_ellipsis", OperatorKeyword_ellipsis },
    { "OperatorKeyword_statementstart", OperatorKeyword_statementstart },
    { "OperatorKeyword_statementend", OperatorKeyword_statementend },
    { "OperatorKeyword_ge", OperatorKeyword_ge },
    { "OperatorKeyword_le", OperatorKeyword_le },
    { "OperatorKeyword_gt", OperatorKeyword_gt },
    { "OperatorKeyword_lt", OperatorKeyword_lt },
    { "OperatorKeyword_eq", OperatorKeyword_eq },
    { "OperatorKeyword_ne", OperatorKeyword_ne },
    { "OperatorKeyword_and", OperatorKeyword_and },
    { "OperatorKeyword_or", OperatorKeyword_or },
    { "OperatorKeyword_not", OperatorKeyword_not },
    { "OperatorKeyword_comma", OperatorKeyword_comma },
    { "OperatorKeyword_endstmt", OperatorKeyword_endstmt },
};

int findHollyKeyword(const char *enumeratorName) {
    for (auto& i : __hollyEnumeratorNames)
        if (!strcmp(i.keyword, enumeratorName))
            return i.id;
    return -1;
}

void InitializeLexerForHolly(Lexer *lexer) {
    lexer->assignVariableToken("[a-zA-Z_-_][_a-zA-Z0-9]*?", Keyword_Name, 0);
    lexer->assignVariableToken("[0-9][0-9]*?.[0-9]*?[a-zA-Z]*?", Keyword_FloatingPoint, 0);
    lexer->assignVariableToken("0x[0-9a-fA-F]*[a-zA-Z]*?", Keyword_Hexadecimal, 0);
    lexer->assignVariableToken("0[0-7]*[0-9a-zA-Z]*?", Keyword_Octal, 1);
    lexer->assignVariableToken("[0-9]*[a-zA-Z]*?", Keyword_Integer, 2);
    lexer->assignVariableToken("\"[\x01-\x09\x0b-\x0c\x0e-\x7f]*?\"", Keyword_Multicharacter, -1); // -1 priority is a special case for string handling
    lexer->assignVariableToken("'[\x01-\x09\x0b-\x0c\x0e-\x7f]*?'", Keyword_Singlecharacter, -1); // -1 priority is a special case for string handling
    for (auto& i : __reservedHollyKeywords)
        lexer->assignConstantToken(i.keyword, i.id);
    for (auto& i : __reservedHollyOperators)
        lexer->assignConstantToken(i.keyword, i.id);
    lexer->enableWhitespaceSkip();
}
