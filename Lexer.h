#pragma once

#include "Token.h"
#include "Regex.h"

struct Lexer {
private:
    std::vector<ConstantToken *> constantTokens;
    std::vector<VariableToken *> variableToken;
    std::string singleLineToken;
    std::string multilineTokenStart, multilineTokenEnd;
    Location curLoc;
    bool whitespaceSkip;
    const char *buf;
    bool err;
    std::vector<Token *> usedTokens;
public:
    Lexer();
    ~Lexer();
    void assignConstantToken(const std::string& token, int id);
    void assignVariableToken(const std::string& grammar, int id, int priority);
    std::pair<int, int> validVariableToken(const std::string& token);
    std::pair<int, int> validConstantToken(const std::string& token);
    void enableWhitespaceSkip();
    void setBuffer(const char *buffer);
    const char *getBuffer() { return buf; }
    void rewindTo(const char *buf);
    Token *findCachedToken(const std::string& str);
    Token *fetchToken();
};