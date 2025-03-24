#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>

class Interpreter {
public:
    Interpreter();
    void run(const std::string &filename);
    void interpretLine(const std::string &line);

private:
    std::unordered_map<std::string, std::string> variables;

    void handlePrintCommand(const std::string &args);
    void handlePrintlnCommand(const std::string &args);
    void handleVariableAssignment(const std::string &name, const std::string &value);
    std::string evaluateExpression(const std::string &expr);
};

#endif // INTERPRETER_H
