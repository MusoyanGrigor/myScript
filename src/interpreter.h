#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

class Interpreter {
public:
    Interpreter();
    void run(const std::string &filename);
    void interpretLine(const std::string &line);

private:
    void handlePrintCommand(const std::string &args);
    void handlePrintlnCommand(const std::string &args);
    std::string getVariableValue(const std::string& varName);
};

#endif
