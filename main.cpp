#include <iostream>

#include "CommandLineInterpreter/CommandLineInterpreter.h"

int main() {
    CommandLineInterpreter cli;
    cli.readCommands(std::cin, std::cout);
    return 0;
}