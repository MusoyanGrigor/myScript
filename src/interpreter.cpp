#include "interpreter.h"
#include <iostream>
#include <stack>
#include <cmath>
#include <algorithm>

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

    if (!line.empty() && line.find('=') != std::string::npos) {
        size_t equal_pos = line.find('=');

        std::string left_side = line.substr(0, equal_pos);
        std::string expression = line.substr(equal_pos + 1);

        left_side.erase(left_side.find_last_not_of(" \t") + 1);
        expression.erase(0, expression.find_first_not_of(" \t"));

        handleVariableAssignment(left_side, expression);
    }

    if (const std::size_t size = command.size();
        command.substr(0, 6) == "print(" && command[size - 1] == ')' && size > 6) {
        // print() command
        std::string args = command.substr(6, size - 7);
        std::getline(stream, args);
        handlePrintCommand(args);
    } else if (command.substr(0, 8) == "println(" && command[size - 1] == ')' && size > 8) {
        // println() command
        std::string args = command.substr(8, size - 9);
        std::getline(stream, args);
        handlePrintlnCommand(args);
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

    if (variables.contains(trimmedValue)) {
        trimmedValue = variables[trimmedValue];
    }
    else if (trimmedValue.find_first_of("+-*/^") != std::string::npos) {
        try {
            trimmedValue = std::to_string(eval(trimmedValue));
        } catch (...) {}
    }
    else if (trimmedValue.front() == '"' && trimmedValue.back() == '"') {
        trimmedValue = trimmedValue.substr(1, trimmedValue.size() - 2);
    }

    variables[name] = trimmedValue;
}

std::string Interpreter::evaluateExpression(const std::string &expr) {
    if (variables.contains(expr) > 0) {
        return variables[expr];
    }
    try {
        return std::to_string(eval(expr));
    } catch (...) {
        return expr;
    }
}

int Interpreter::precedence(const char op) {
    switch (op) {
        case '+':
        case '-': return 1;
        case '*':
        case '/': return 2;
        case '^': return 3;
        default: return 0;
    }
}

double Interpreter::applyOp(const double num1, const double num2, const char op) {
    switch (op) {
        case '+': return num1 + num2;
        case '-': return num1 - num2;
        case '*': return num1 * num2;
        case '/':
            if (num2 == 0) {
                throw std::runtime_error("Error: Division by zero");
            }
            return num1 / num2;
        case '^': return pow(num1, num2);
        default:
            throw std::invalid_argument("Invalid operator");
    }
}

double Interpreter::eval(const std::string &expression) {
    std::stack<double> values;
    std::stack<char> ops;

    for (size_t i = 0; i < expression.length(); i++) {
        if (isspace(expression[i])) {
            continue;
        }

        if (isdigit(expression[i])) {
            double val = 0;
            while (i < expression.length() && isdigit(expression[i])) {
                val = val * 10 + (expression[i] - '0');
                i++;
            }
            i--;
            values.push(val);
        } else if (expression[i] == '(') {
            ops.push(expression[i]);
        } else if (expression[i] == ')') {
            while (!ops.empty() && ops.top() != '(') {
                const double val2 = values.top();
                values.pop();
                const double val1 = values.top();
                values.pop();
                const char op = ops.top();
                ops.pop();
                values.push(applyOp(val1, val2, op));
            }
            ops.pop();
        } else {
            while (!ops.empty() && precedence(ops.top()) >= precedence(expression[i])) {
                const double val2 = values.top();
                values.pop();
                const double val1 = values.top();
                values.pop();
                const char op = ops.top();
                ops.pop();
                values.push(applyOp(val1, val2, op));
            }
            ops.push(expression[i]);
        }
    }

    while (!ops.empty()) {
        const double val2 = values.top();
        values.pop();
        const double val1 = values.top();
        values.pop();
        const char op = ops.top();
        ops.pop();
        values.push(applyOp(val1, val2, op));
    }
    return values.top();
}