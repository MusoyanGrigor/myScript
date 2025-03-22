#include "interpreter.h"
#include <iostream>
#include <sstream>

Interpreter::Interpreter() {}

void Interpreter::run(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        interpretLine(line);
    }

    file.close();
}

void Interpreter::interpretLine(const std::string &line) {
    std::istringstream stream(line);
    std::string command;
    stream >> command;

    const std::size_t size = command.size();

    if (command.size() > 6 && command.substr(0, 6) == "print(" && command[command.size() - 1] == ')') {
        std::string args = command.substr(6, command.size() - 7);
        std::getline(stream, args);
        handlePrintCommand(args);
    } else if (command.size() > 8 && command.substr(0,8) == "println(" && command[command.size() - 1] == ')') {
      std::string args = command.substr(8, command.size() - 9);
      std::getline(stream, args);
      handlePrintlnCommand(args);
    } else {
        std::cerr << "Syntax error: " << command << std::endl;
    }
}

void Interpreter::handlePrintCommand(const std::string &args) {
    std::istringstream argStream(args);
    std::string part;
    bool first = true;

    while (std::getline(argStream, part, ',')) {
        part.erase(0, part.find_first_not_of(" \t"));
        part.erase(part.find_last_not_of(" \t") + 1);

        if (!first) {
            std::cout << " ";
        }

        first = false;
        if (part.front() == '"' && part.back() == '"') {
            std::cout << part.substr(1, part.size() - 2);
        } else {
            ; //std::cout << getVariableValue(part);
        }
    }
}

void Interpreter::handlePrintlnCommand(const std::string &args) {
    handlePrintCommand(args);
    std::cout <<std::endl;
}
