#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include <algorithm>
#include <optional>

#include "Lexer.h"

#include "HollyLanguage.h"

struct ParserFragmentRule {
private:
public:
};

struct ParserRule {
private:
    std::string name;
    std::vector<std::string> syntax;
    friend struct Parser;
public:
    void setName(const std::string& name);
    void addSyntax(const std::string& syntax);
};

void ParserRule::setName(const std::string& name) {
    this->name = name;
}

void ParserRule::addSyntax(const std::string& syntax) {
    this->syntax.push_back(syntax);
}

struct Parser {
private:
    std::vector<ParserRule *> rules;
public:
    Parser();
    void addRule(ParserRule *r);
    ParserRule *findRule(const std::string& rule);
    bool runRule(const std::string& rule, Lexer *l);
};

Parser::Parser() {
}

void Parser::addRule(ParserRule *r) {
    rules.push_back(r);
}

ParserRule *Parser::findRule(const std::string& rule) {
    for (auto& i : rules)
        if (i->name == rule)
            return i;
    return nullptr;
}

bool Parser::runRule(const std::string& rule_, Lexer *l) {
    ParserRule *rule = findRule(rule_);
    Lexer grammarLexer;
    grammarLexer.enableWhitespaceSkip();
    grammarLexer.assignConstantToken("E", 2);
    grammarLexer.assignVariableToken("@[a-zA-Z0-9_-_]*", 0, 0);
    grammarLexer.assignVariableToken("[a-zA-Z0-9_-_]*", 1, 0);
    printf("running %s\n", rule_.c_str());

    const char *buf = nullptr;
    bool state = false;
    for (auto& i : rule->syntax) {
        grammarLexer.setBuffer(i.c_str());
        Token *t = nullptr;
        while (true) {
            t = grammarLexer.fetchToken();
            if (!t)
                break;
            
            switch (t->id) {
            case 0:
                buf = l->getBuffer();
                switch (auto id = findHollyKeyword(t->token.c_str() + 1)->id) {
                case Keyword_Integer:
                    t = l->fetchToken();                    
                    if (t && id == t->id) {
                        return true;
                    }
                    break;
                case OperatorKeyword_add:
                case OperatorKeyword_star:
                case OperatorKeyword_exprbegin:
                case OperatorKeyword_exprend:
                    t = l->fetchToken();
                    if (t && id == t->id) {
                        state = true;
                        continue;
                    }
                    l->rewindTo(buf);
                    return false;
                default:
                    printf("WTF %s\n", t->token.c_str());
                    std::exit(0);
                }

                l->rewindTo(buf);
                break;
            case 2:
                printf("hallo\n");
                break;
            case 1:
                runRule(t->token, l);
                break;
            }
        }

        if (state)
            return true;
    }
    return false;
}

int main() {
    Lexer l;
    Parser p;
    ParserRule exprRule;
    ParserRule termListRule;
    ParserRule termRule;
    ParserRule factorRule;
    ParserRule factorListRule;
    InitializeLexerForHolly(&l);

    exprRule.setName("expr");
    exprRule.addSyntax("term termList");
    termListRule.setName("termList");
    //termListRule.addSyntax("@OperatorKeyword_minus term termList");
    termListRule.addSyntax("@OperatorKeyword_add term termList");
    termListRule.addSyntax("E");
    termRule.setName("term");
    termRule.addSyntax("factor factorList");
    factorRule.setName("factor");
    factorRule.addSyntax("@Keyword_Integer");
    factorRule.addSyntax("@OperatorKeyword_exprbegin expr @OperatorKeyword_exprend");
    factorListRule.setName("factorList");
    factorListRule.addSyntax("@OperatorKeyword_star factor factorList");
    factorListRule.addSyntax("E");

    p.addRule(&exprRule);
    p.addRule(&termListRule);
    p.addRule(&termRule);
    p.addRule(&factorRule);
    p.addRule(&factorListRule);

    l.setBuffer("1+1*(4*4)");

    //expr->term->factor->1->factorList->none->termList->+term->factor->2->factorList->none->termList->none

    p.runRule("expr", &l);
    return 0;
}