#include "interpreter.h"

int main() {
    Interpreter interpreter;
    const std::string filePath = "../script.my";
    interpreter.run(filePath);
    
    return 0;
}
