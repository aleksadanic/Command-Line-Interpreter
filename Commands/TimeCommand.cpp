#include "TimeCommand.h"

#include <ctime>

#include "../Exceptions/AllExceptions.h"

void TimeCommand::validate() {
    if (commandData.arguments.size() > 0) {
        throw SemanticError("arguments not expected", commandData.arguments[0].position);
    }
    if (commandData.options.size() > 0) {
        throw SemanticError("option not expected", commandData.options[0].position);
    }
}

void TimeCommand::execute(std::istream& input, std::ostream& output, CommandLineInterpreter& cli) {
    std::time_t time = std::time(0);
    std::tm* now = std::localtime(&time);
    if (now->tm_hour < 10) {
        output << '0';
    }
    output << now->tm_hour << ':';
    if (now->tm_min < 10) {
        output << '0';
    }
    output << now->tm_min << ':';
    if (now->tm_sec < 10) {
        output << '0';
    }
    output << now->tm_sec;
}