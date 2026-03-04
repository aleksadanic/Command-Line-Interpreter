#include "HeadCommand.h"

#include "../Exceptions/AllExceptions.h"

void HeadCommand::validate() {
    if (commandData.arguments.size() > 1) {
        throw SemanticError("too many arguments", commandData.arguments[1].position);
    }
    if (commandData.options.size() == 0) {
        throw SemanticError("option not specified", -1);
    }
    if (commandData.options.size() > 1) {
        throw SemanticError("too many options", commandData.options[1].position);
    }
    if (commandData.options[0].content.size() <= 1 || commandData.options[0].content[0] != 'n') {
        throw SemanticError("option wrongly specified", commandData.options[0].position);
    }
    if (commandData.options[0].content.size() > 6) {
        throw SemanticError("too many digits", commandData.options[0].position + 7);
    }
    for (int i = 1; i < (int)commandData.options[0].content.size(); i++) {
        if (commandData.options[0].content[i] < '0' || commandData.options[0].content[i] > '9') {
            throw SemanticError("number was expected", commandData.options[0].position + i + 1);
        }
    }
}

void HeadCommand::prepare() {
    if (commandData.arguments.size() != 0) {
        processArgument(commandData.arguments[0]);
    }
}

void HeadCommand::execute(std::istream& input, std::ostream& output, CommandLineInterpreter& cli) {
    int n = 0;
    for (int i = 1; i < (int)commandData.options[0].content.size(); i++) {
        n = 10 * n + (int)(commandData.options[0].content[i] - '0');
    }
    if (commandData.arguments.size() == 0) {
        std::stringstream ss;
        char ch;
        while (input.get(ch)) {
            ss << ch;
        }
        std::string str;
        while (n-- && getline(ss, str)) {
            output << str << '\n';
        }
    } else {
        for (int i = 0; i < (int)commandData.arguments[0].content.size() && n > 0; i++) {
            if (commandData.arguments[0].content[i] == '\n') {
                n--;
            }
            output << commandData.arguments[0].content[i];
        }
    }
}