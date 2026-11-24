#include <algorithm>

#include "Lexer.h"

Lexer::Lexer() {
    curLoc.col = 0;
    curLoc.line = 0;
    curLoc.index = 0;
    buf = nullptr;
    whitespaceSkip = false;
    err = false;
}

Lexer::~Lexer() {
    for (auto& i : constantTokens) if (i) delete i;
    for (auto& i : variableToken) if (i) delete i;
    for (auto& i : usedTokens) if (i) delete i;
}

void Lexer::assignConstantToken(const std::string& token, int id) {
    constantTokens.push_back(new ConstantToken{ token, id });
    std::sort(constantTokens.begin(), constantTokens.end(), [](const ConstantToken *a, const ConstantToken *b) { return a->token.length() > b->token.length(); });
}

void Lexer::assignVariableToken(const std::string& grammar, int id, int priority) {
    VariableToken *tok = new VariableToken;
    const char *p = grammar.c_str();
    bool ingrammar = false;
    std::string rule;

    tok->rule = grammar;
    tok->id = id;
    tok->priority = priority;
    ConstantStringExecutor *exec = nullptr;
    RangeStringExecutor *ranged = nullptr;
    while (*p != '\0') {
        if (*p == '[' && *(p - 1) != '\\' && !ingrammar) {
            if (rule.length() != 0) {
                exec = new ConstantStringExecutor;
                exec->setConstStringRule(rule);
                tok->exec.push_back(exec);
                exec->setOptional(false);
            }

            rule = "";
            ingrammar = true;
            ranged = new RangeStringExecutor;
        }

        if (!ingrammar) {
            if (*(p + 1) == '\\' && *p == '\\')
                p++;
            rule += *p;
        }

        if (ingrammar) {
            std::string str;
            if (*p == '-') {
                str = *(p - 1) + std::string("-") + *(p + 1);
                ranged->addRange(str);
                p += 2;
                continue;
            }
        }

        if (*p == ']' && *(p - 1) != '\\' && ingrammar) {
            rule = "";
            if (*(p + 1) == '*') {
                ranged->setLoop(true);
                p++;
            } else
                ranged->setLoop(false);

            if (*(p + 1) == '?') {
                p++;
                ranged->setOptional(true);
            } else {
                ranged->setOptional(false);
            }
            tok->exec.push_back(ranged);
            ranged = nullptr;
            ingrammar = false;
        }

        p++;
    }

    if (rule.length() != 0) {
        exec = new ConstantStringExecutor;
        exec->setConstStringRule(rule);
        exec->setOptional(false);
        tok->exec.push_back(exec);
    }
    variableToken.push_back(tok);

    std::sort(variableToken.begin(), variableToken.end(), [](const VariableToken *a, const VariableToken *b) { return a->priority < b->priority; });
}

std::pair<int, int> Lexer::validVariableToken(const std::string& token) {
    bool valid = false;
    int sliced = 0;
    int id = -1;
    for (auto& i : variableToken) {
        sliced = 0;
        for (int j = 0; j < i->exec.size(); j++) {
            std::string str = token.substr(sliced);
            int s = i->exec[j]->execute(str);
            sliced += s;
            const char *ptr = token.c_str() + 1;
            // special case
            if (j == i->exec.size() - 1 && i->priority == -1) {
                const char c = token[0];
                const char *str = ptr;
                if (i->rule.c_str()[0] != c)
                    break;

                while (*ptr != '\0' && *ptr != c) {
                    if (*ptr == '\n') {
                        printf("did not expected newline on %c", c);
                        sliced = 0;
                        valid = false;
                        break;
                    }

                    if (*ptr == '\\') {
                        ptr += 2;
                    } else
                        ptr++;
                }

                sliced = ptr - str + 2;
                valid = true;
                break;
            }
            if (s != 0)
                valid = true;
            else {
                if (i->exec[j]->isOptional()) {
                    continue;
                }
                valid = false;
                break;
            }
        }

        if (valid) {
            id = i->id;
            break;
        }
    }

    if (!valid)
        return { -1, -1 };
    return { id, sliced };
}

std::pair<int, int> Lexer::validConstantToken(const std::string& token) {
    int counter = 0;
    int slice = 0;
    for (auto& i : constantTokens) {
        bool valid = false;

        for (int j = 0; j < token.length() && j < i->token.length()
            && token.length() >= i->token.length() /* didn't add this forgot to but good reminder, will fuck up the lexing if not there */; j++) {
            if (token[j] == i->token[j]) {
                valid = true;
            } else {
                valid = false;
                break;
            }
        }

        if (valid) {
            return { i->id, (int)i->token.length() };
        }
    }
    return { -1, -1 };
}

void Lexer::enableWhitespaceSkip() {
    whitespaceSkip = true;
}

void Lexer::setBuffer(const char *buffer) {
    buf = buffer;
}

void Lexer::rewindTo(const char *buf) {
    setBuffer(buf);
}

Token *Lexer::findCachedToken(const std::string& str) {
    for (auto& i : usedTokens)
        if (i->token == str)
            return i;
    return nullptr;
}

Token *Lexer::fetchToken() {
    Token *t;
    if (!buf || *buf == '\0')
        return nullptr;

    if (whitespaceSkip) {
        while (isspace(*buf))
            buf++;
    }

    if (*buf == '\0')
        return nullptr;

    auto valid = validVariableToken(buf);
    if (valid.first == -1) {
        valid = validConstantToken(buf);
        if (valid.first == -1) {
            printf("invalid character '%c' digit %d\n", *buf, *buf);
            return nullptr;
        }
    } else {
        auto validconstant = validConstantToken(buf);
        if (validconstant.second >= valid.second)
            valid = validconstant;
    }

    std::string str;
    for (int i = 0; i < valid.second; i++)
        str += buf[i];

    if ((t = findCachedToken(str)) != nullptr) {
        buf += valid.second;
        return t;
    }

    t = new Token;
    t->id = valid.first;
    t->token = str;
    buf += valid.second;
    usedTokens.push_back(t);
    return t;
}