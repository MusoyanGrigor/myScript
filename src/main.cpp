#include "interpreter.h"

int main() {

    const std::string filePath = "../script.my";
    Interpreter::run(filePath);
    
    return 0;
}
