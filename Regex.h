#pragma once

#include <string>
#include <vector>

struct VariableTokenExecutor {
private:
    bool optional;
public:
    void setOptional(bool state) { optional = state; }
    bool isOptional() { return optional; }
    virtual int getType() = 0;
    virtual int execute(const std::string& str) = 0;
};

struct ConstantStringExecutor : public VariableTokenExecutor {
private:
    std::string conststring;
public:
    int getType() { return 0; }
    void setConstStringRule(const std::string& str);
    int execute(const std::string& str) override;
};

struct RangeStringExecutor : public VariableTokenExecutor {
private:
    std::vector<std::string> range;
    bool loop;
public:
    int getType() { return 1; }
    void addRange(const std::string& range);
    void setLoop(bool state);
    int execute(const std::string& str) override;
};

struct VariableToken {
    std::string rule;
    int id;
    int priority;
    std::vector<VariableTokenExecutor *> exec;
};

