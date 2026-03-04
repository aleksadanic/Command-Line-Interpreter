#include <iostream>

#include "CommandLineInterpreter.h"
#include "../Pipeline/Pipe.h"
#include "../Exceptions/AllExceptions.h"

void CommandLineInterpreter::readCommands(std::istream& in, std::ostream& out) {
    while (true) {
        if (&in == &std::cin) {
            std::cout << prompt << ' ';
        }
        std::string line;
        if (!getline(in, line)) {
            break;
        }
        if (line.size() > 512) {
            line.resize(512);
        }
        Pipe pipe;
        try {
            pipe.parse(line);
            pipe.validate();
            pipe.execute(std::cin, out, *this);
            std::cin.clear();
        } catch (Exception& e) {
            if (e.getPos() >= 0) {
                for (int i = 0; i < (int)(prompt.size() + 1 + e.getPos()); i++) {
                    std::cerr << ' ';
                }
                std::cerr << "^\n";
            }
            std::cerr << e.what() << '\n';
        }
    }
}