#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

#include "Lexer.h"

#include "HollyLanguage.h"

#include "Parser.h"

void debug(ParseTree *tree, int depth = 0) {
    if (tree == nullptr)
        return;

    for (int i = 0; i < depth; i++)
        printf("  ");

    printf("- %s%s\n", tree->production.c_str(), tree->terminal ? (" (" + tree->terminal->token + ")").c_str() : "");
    for (ParseTree *c : tree->children) {
        debug(c, depth + 1);
    }
}

static void InitializeParserForHolly(Parser *p) {
    std::fstream f;
    std::string str;
    int left = 0;
    f.open("HollyGrammar.grammar", std::ios_base::in | std::ios_base::binary);

    Lexer grammarLexerForHolly;
    grammarLexerForHolly.enableWhitespaceSkip();
    grammarLexerForHolly.assignConstantToken("E", 2);
    grammarLexerForHolly.assignVariableToken("@[a-zA-Z0-9_-_]*", 0, 0);
    grammarLexerForHolly.assignVariableToken("[a-zA-Z0-9_-_#-#]*", 1, 0);

    ParserRule rule;
    Token *t;
    while (std::getline(f, str)) {
        if (str[0] == ';')
            continue;

        if (isspace(str[0]))
            continue;

        grammarLexerForHolly.setBuffer(str.c_str());
        if (left == 0) {
            rule = {};
            rule.setName(grammarLexerForHolly.fetchToken()->token);
            left = std::atoi(grammarLexerForHolly.fetchToken()->token.c_str());
        } else {
            std::vector<std::string> rules;
            while (t = grammarLexerForHolly.fetchToken())
                rules.push_back(t->token);
            rule.addSyntax(rules);
            left--;
            if (left == 0)
                p->addRule(&rule);
        }
    }
}

static std::pair<std::string, std::vector<char>> readFile(const std::string& file) {
    std::fstream f(file, std::ios_base::in | std::ios_base::binary);
    std::vector<char> buf;
    std::string debugtype;
    if (!f)
        return {};

    f.seekg(0, std::ios_base::end);
    auto sz = (int)f.tellg();
    f.seekg(0, std::ios_base::beg);
    std::getline(f, debugtype);
    buf.resize(sz);
    f.read(&buf[0], sz);
    f.close();
    return { debugtype, buf };
}

int main(int argc, char *argv[]) {
    Lexer l;
    Parser p;

    InitializeLexerForHolly(&l);
    InitializeParserForHolly(&p);
    p.setKeywordCallback(findHollyKeyword);
    auto test = readFile("Tree1.holly");
    l.setBuffer(&test.second[0]);
    debug(p.parse(test.first, &l));
    return 0;
}