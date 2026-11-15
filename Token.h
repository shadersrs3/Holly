#pragma once

#include <string>

struct ConstantToken {
    std::string token;
    int id;
};

struct Location {
    int line, col;
    int index;
};

struct Token {
    std::string token;
    int id;
    Location loc;
};