#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include <algorithm>
#include <optional>
#include <iostream>

#include "Lexer.h"

#include "HollyLanguage.h"

#include "Parser.h"

void debug(ParseTree *tree, int depth = 0) {
    if (tree == nullptr)
        return;

    for (int i = 0; i < depth; i++)
        printf("\t");

    printf("%s%s\n", tree->production.c_str(), tree->terminal ? (" (" + tree->terminal->token + ")").c_str() : "");
    for (ParseTree *c : tree->children) {
        debug(c, depth + 1);
    }
}

int main() {
    Lexer l;
    Parser p;
    ParserRule exprRule;
    ParserRule termList;
    ParserRule term;
    ParserRule factor;
    ParserRule factorList;
    InitializeLexerForHolly(&l);
    exprRule.setName("expr");
    exprRule.addSyntax({ "#term", "termList" });

    termList.setName("termList");
    termList.addSyntax({ "@OperatorKeyword_plus", "#term", "termList" });
    termList.addSyntax({ "@OperatorKeyword_minus", "#term", "termList" });
    termList.addSyntax({ "E" });
    term.setName("term");
    term.addSyntax({ "factor", "factorList" });
    factorList.setName("factorList");
    factorList.addSyntax({ "@OperatorKeyword_star", "factor", "factorList" });
    factorList.addSyntax({ "E" });
    factor.setName("factor");
    factor.addSyntax({ "@Keyword_Integer" });
    factor.addSyntax({ "@OperatorKeyword_exprbegin", "expr", "@OperatorKeyword_exprend" });
    p.addRule(&termList);
    p.addRule(&term);
    p.addRule(&factor);
    p.addRule(&factorList);
    p.addRule(&exprRule);

    l.setBuffer("+++1 + 1");
    p.setKeywordCallback(findHollyKeyword);
    ParseTree *t = p.parse("expr", &l);
    debug(t);
    return 0;
}