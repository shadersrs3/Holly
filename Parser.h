#pragma once

#include "ParseTree.h"
#include "ParserRule.h"
#include "Lexer.h"

typedef int (*ParserKeywordCallback)(const char *);

struct Parser {
private:
    Lexer grammarLexer;
    ParserKeywordCallback keywordCallback;
    std::vector<ParserRule *> rules;
public:
    Parser();
    void addRule(ParserRule *r);
    ParserRule *findRule(const std::string& rule);
    void setKeywordCallback(ParserKeywordCallback keywordCallback);
    ParseTree *parse(const std::string& symbol, Lexer *l);
};