#include <cstdio>

#include "Parser.h"

void ParserRule::setName(const std::string& name) {
    this->name = name;
}

void ParserRule::addSyntax(const std::vector<std::string>& syntax) {
    this->syntax.push_back(syntax);
}

Parser::Parser() {
    grammarLexer.enableWhitespaceSkip();
    grammarLexer.assignConstantToken("E", 2);
    grammarLexer.assignVariableToken("@[a-zA-Z0-9_-_]*", 0, 0);
    grammarLexer.assignVariableToken("[a-zA-Z0-9_-_#-#]*", 1, 0);
    keywordCallback = nullptr;
}

void Parser::addRule(ParserRule *r) {
    rules.push_back(*r);
}

ParserRule *Parser::findRule(const std::string& rule) {
    for (auto& i : rules)
        if (i.getName() == rule)
            return &i;
    return nullptr;
}

void Parser::setKeywordCallback(ParserKeywordCallback keywordCallback) {
    this->keywordCallback = keywordCallback;
}

ParseTree *Parser::parse(const std::string& symbol, Lexer *l) {
    Token *t;
    ParseTree *parseTree = new ParseTree;
    parseTree->production = symbol;
    parseTree->terminal = nullptr;

    std::vector<ParseTree *> children;
    grammarLexer.setBuffer(symbol.c_str());
    t = grammarLexer.fetchToken();
    switch (t->id) {
    case 0: {
        const char *buf = l->getBuffer();
        Token *tok = l->fetchToken();
        if (!tok)
            return nullptr;

        if (tok->id == keywordCallback(t->token.c_str() + 1)) {
            parseTree->production = symbol;
            parseTree->terminal = tok;
            return parseTree;
        } else {
            l->setBuffer(buf);
        }
        delete parseTree;
        return nullptr;
    }
    case 1: {
        ParserRule *rule;
        if (t->token[0] == '#') {
            rule = findRule(t->token.c_str() + 1);
        } else {
            rule = findRule(t->token);
        }

        ParseTree *tree = nullptr;
        if (!rule) {
            printf("no rule found %s\n", t->token.c_str());
            delete parseTree;
            return nullptr;
        }

        for (auto& i : rule->getSyntax()) {
            bool success = true;
            const char *buffer = l->getBuffer();

            children.clear();
            for (auto& j : i) {
                tree = parse(j, l);
                if (!tree && !(j[0] == '#')) {
                    success = false;
                    break;
                }

                children.push_back(tree);
            }

            if (success) {
                parseTree->children = children;
                return parseTree;
            }

            for (auto& i : children)
                if (i) delete i;

            l->setBuffer(buffer);
        }

        break;
    }
    case 2:
        parseTree->production = "E";
        return parseTree;
    }

    delete parseTree;
    return nullptr;
}
