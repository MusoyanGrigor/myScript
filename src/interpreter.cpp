#include <iostream>
#include <sstream>
#include <vector>
#include "interpreter.h"

Interpreter::Interpreter() = default;

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
    static std::vector<std::string> variableInfo;

    if (command.substr(0, 6) == "print(" && command[size - 1] == ')' && size > 6) {
        std::string args = command.substr(6, size - 7);
        std::getline(stream, args);
        handlePrintCommand(args);
    } else if (command.substr(0, 8) == "println(" && command[size - 1] == ')' && size > 8) {
        std::string args = command.substr(8, size - 9);
        std::getline(stream, args);
        handlePrintlnCommand(args);
    } else if (size > 0 && std::isalpha(command[0]) ) {
        std::string args;
        std::getline(stream, args);
        variableInfo = handleVariableCommand(args);
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
            std::vector<std::string> variableInfo = handleVariableCommand(args);
            std::cout << variableInfo.at(1) << " ";
        }
    }
}

void Interpreter::handlePrintlnCommand(const std::string &args) {
    handlePrintCommand(args);
    std::cout << std::endl;
}

std::vector<std::string> Interpreter::handleVariableCommand(const std::string &args) {
    std::istringstream argStream(args);
    std::string part;

    std::string variableName;
    std::string variableValue;
    std::string variableType;

    size_t partIndex = 0;
    while (std::getline(argStream, part, '=')) {
        if (partIndex == 0) {
            variableName = part;
        } else if (partIndex == 1) {
            variableValue = part;
        }

        ++partIndex;
    }
    try {
        std::stof(variableValue);
        variableType = "number";
    } catch (const std::invalid_argument &) {
        variableType = "string";
    }
    std::vector<std::string> variableInfo = {variableName, variableValue, variableType};

    return variableInfo;
}