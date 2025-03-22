#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <string>
#include <fstream>

class Interpreter {
public:
    Interpreter();
    static void run(const std::string &filename);
    static void interpretLine(const std::string &line);

private:
    static void handlePrintCommand(const std::string &args);
    static void handlePrintlnCommand(const std::string &args);
    std::string getVariableValue(const std::string& varName);
};

#endif
