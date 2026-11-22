#pragma once

#include <string>
#include <vector>

#include "Token.h"

struct ParseTree {
    std::string production;
    Token *terminal;
    std::vector<ParseTree *> children;
};
