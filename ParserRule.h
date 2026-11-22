#pragma once

#include <string>
#include <vector>

struct ParserRule {
private:
    std::string name;
    std::vector<std::vector<std::string>> syntax;
public:
    const std::string& getName() const { return name; }
    void setName(const std::string& name);
    void addSyntax(const std::vector<std::string>& syntax);
    inline std::vector<std::vector<std::string>>& getSyntax() { return syntax; }
};