#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>

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

static void InitializeParserForHolly(Parser *p, const std::string& path) {
    std::fstream f;
    std::string str;
    int left = 0;
    f.open(path, std::ios_base::in | std::ios_base::binary);

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

        while (isspace(str[0])) {
            str = str.substr(1);
            continue;
        }

        if (str.length() == 0)
            continue;

        if (str.length() >= 2 && str[0] == '-' && str[1] == '-') {
            p->addRule(&rule);
            left = 0;
            continue;
        }

        grammarLexerForHolly.setBuffer(str.c_str());
        if (left == 0) {
            rule = {};
            rule.setName(grammarLexerForHolly.fetchToken()->token);
            left = 1;
        } else {
            std::vector<std::string> rules;
            while (t = grammarLexerForHolly.fetchToken())
                rules.push_back(t->token);
            rule.addSyntax(rules);
        }
    }
}


static void InitializeLexerForHolly(Lexer *l, const std::string& path) {
    std::fstream f;
    std::string str;
    int left = 0;
    f.open(path, std::ios_base::in | std::ios_base::binary);
    Lexer lex;
    lex.enableWhitespaceSkip();
    lex.assignConstantToken("E", 2);
    lex.assignVariableToken("[\x21-\x7E]*", 0, 0);
    size_t id = __hollyEnumeratorNames.size();
    while (std::getline(f, str)) {
        if (str[0] == '#')
            continue;

        if (isspace(str[0]))
            continue;

        lex.setBuffer(str.c_str());
        Token *type = lex.fetchToken();
        Token *keyword = lex.fetchToken();
        if (auto id_ = FindHollyKeyword(type->token.c_str()); id_ != -1) {
            __hollyEnumeratorNames.push_back({ id_, type->token, keyword->token });
            l->assignConstantToken(keyword->token, id_);
        } else {
            __hollyEnumeratorNames.push_back({ (int)id, type->token, keyword->token });
            l->assignConstantToken(keyword->token, (int)id);
            id++;
        }
    }
}

static std::pair<std::string, std::vector<char>> readHollyFile(const std::string& file) {
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
    InitializeLexerForHollyStartup(&l);
    InitializeParserForHolly(&p, "HollyGrammar.grammar");
    InitializeLexerForHolly(&l, "HollyConstants.cnst");
    p.setKeywordCallback(FindHollyKeyword);
    auto test = readHollyFile("Tree1.holly");
    l.setBuffer(&test.second[0]);
    debug(p.parse(test.first, &l));
    return 0;
}