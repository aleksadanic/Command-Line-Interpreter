#include "EchoCommand.h"

#include <sstream>

#include "../Util/Util.h"
#include "../Exceptions/AllExceptions.h"

void EchoCommand::validate() {
    if (commandData.arguments.size() > 1) {
        throw SemanticError("too many arguments", commandData.arguments[1].position);
    }
    if (commandData.options.size() > 0) {
        throw SemanticError("option not expected", commandData.options[0].position);
    }
}

void EchoCommand::prepare() {
    if (commandData.arguments.size() != 0) {
        processArgument(commandData.arguments[0]);
    }
}

void EchoCommand::execute(std::istream& input, std::ostream& output, CommandLineInterpreter& cli) {
    if (commandData.arguments.size() == 0) {
        std::stringstream ss;
        char ch;
        while (input.get(ch)) {
            ss << ch;
        }
        output << ss.str();
    } else {
        output << commandData.arguments[0].content;
    }
}