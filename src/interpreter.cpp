#include "interpreter.h"
#include <iostream>

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

    if (command.substr(0, 6) == "print(" && command[size - 1] == ')' && size > 6) {
        std::string args = command.substr(6, size - 7);
        std::getline(stream, args);
        handlePrintCommand(args);
    } else if (command.substr(0, 8) == "println(" && command[size - 1] == ')' && size > 8) {
        std::string args = command.substr(8, size - 9);
        std::getline(stream, args);
        handlePrintlnCommand(args);
    } else if (std::isalpha(command[0]) && size > 0) {
        std::string rest;
        std::getline(stream, rest);
        const size_t equalPos = rest.find('=');

        if (equalPos != std::string::npos) {
            std::string varName = command;
            std::string value = rest.substr(equalPos + 1);

            varName.erase(varName.find_last_not_of(" \t") + 1);
            value.erase(0, value.find_first_not_of(" \t"));

            handleVariableAssignment(varName, value);
        } else {
            std::cerr << "Syntax error: " << command << std::endl;
        }
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
            std::cout << evaluateExpression(part);
        }
    }
}

void Interpreter::handlePrintlnCommand(const std::string &args) {
    handlePrintCommand(args);
    std::cout << std::endl;
}

void Interpreter::handleVariableAssignment(const std::string &name, const std::string &value) {
    std::string trimmedValue = value;
    if (trimmedValue.front() == '"' && trimmedValue.back() == '"') {
        trimmedValue = trimmedValue.substr(1, trimmedValue.size() - 2);
    }
    else {
        trimmedValue = std::to_string(eval(trimmedValue));
    }

    variables[name] = trimmedValue;
}

std::string Interpreter::evaluateExpression(const std::string &expr) {
    if (variables.contains(expr)) {
        return variables[expr];
    }
    return expr;
}

double Interpreter::eval(const std::string& expression) {
    std::stringstream ss(expression);
    double result, num;
    char op;

    ss >> result;
    while (ss >> op >> num) {
        if (op == '+') result += num;
        else if (op == '-') result -= num;
        else if (op == '*') result *= num;
        else if (op == '/') result /= num;
    }
    return result;
}