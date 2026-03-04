#include "WcCommand.h"

#include "../Util/Util.h"
#include "../Exceptions/AllExceptions.h"

void WcCommand::validate() {
    if (commandData.options.size() == 0) {
        throw SemanticError("option not specified", -1);
    }
    if (commandData.options.size() > 1) {
        throw SemanticError("too many options", commandData.options[1].position);
    }
    if (commandData.options[0].content != "w" && commandData.options[0].content != "c") {
        throw SemanticError("option " + commandData.options[0].content + " doesn't exist", commandData.options[0].position);
    }
    if (commandData.arguments.size() > 1) {
        throw SemanticError("too many arguments", commandData.arguments[1].position);
    }
}

void WcCommand::prepare() {
    if (commandData.arguments.size() != 0) {
        processArgument(commandData.arguments[0]);
    }
}

void WcCommand::execute(std::istream& input, std::ostream& output, CommandLineInterpreter& cli) {
    std::string data = "";
    if (commandData.arguments.size() == 0) {
        char ch;
        while (input.get(ch)) {
            data += ch;
        }
    } else {
        data = commandData.arguments[0].content;
    }
    if (commandData.options[0].content == "w") {
        output << util::wordsCount(data);
    } else {
        output << util::charactersCount(data);
    }
}