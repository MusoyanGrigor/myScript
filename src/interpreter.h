#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <string>
#include <fstream>
#include <vector>

class Interpreter {
public:
    Interpreter();
    static void run(const std::string &filename);
    static void interpretLine(const std::string &line);

private:
    static void handlePrintCommand(const std::string &args);
    static void handlePrintlnCommand(const std::string &args);
    static std::vector<std::string> handleVariableCommand(const std::string &args);
};

#endif // INTERPRETER_H
