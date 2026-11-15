#pragma once

enum KeywordId {
    Keyword_Name,
    Keyword_Octal,
    Keyword_Integer,
    Keyword_Hexadecimal,
    Keyword_FloatingPoint,
    Keyword_Multicharacter,
    Keyword_Singlecharacter,
    ConstKeyword_datastruct,
    ConstKeyword_sharedstruct,
    ConstKeyword_int8,
    ConstKeyword_int16,
    ConstKeyword_int32,
    ConstKeyword_int64,
    ConstKeyword_uint8,
    ConstKeyword_uint16,
    ConstKeyword_uint32,
    ConstKeyword_uint64,
    ConstKeyword_void,
    ConstKeyword_alias,
    OperatorKeyword_pointerstructaccess,
    OperatorKeyword_structaccess,
    OperatorKeyword_exprbegin,
    OperatorKeyword_exprend,
    OperatorKeyword_add,
    OperatorKeyword_minus,
    OperatorKeyword_star,
    OperatorKeyword_divide,
    OperatorKeyword_modulo,
    OperatorKeyword_bitand,
    OperatorKeyword_bitxor,
    OperatorKeyword_bitor,
    OperatorKeyword_bitcompl,
    OperatorKeyword_shr,
    OperatorKeyword_shl,
    OperatorKeyword_assignequal,
    OperatorKeyword_bracketstart,
    OperatorKeyword_bracketend,
    OperatorKeyword_ellipsis,
    OperatorKeyword_statementstart,
    OperatorKeyword_statementend,
    OperatorKeyword_ge,
    OperatorKeyword_le,
    OperatorKeyword_gt,
    OperatorKeyword_lt,
    OperatorKeyword_eq,
    OperatorKeyword_ne,
    OperatorKeyword_and,
    OperatorKeyword_or,
    OperatorKeyword_not,
    OperatorKeyword_comma,
    OperatorKeyword_endstmt,
};

struct ConstKeyword {
    const char *keyword;
    int id;
};

extern ConstKeyword __reservedHollyKeywords[];
extern ConstKeyword __reservedHollyOperators[];
extern ConstKeyword __hollyEnumeratorNames[];
ConstKeyword *findHollyKeyword(const char *enumeratorName);