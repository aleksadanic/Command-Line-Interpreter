#include "DateCommand.h"

#include <ctime>

#include "../Exceptions/AllExceptions.h"

void DateCommand::validate() {
    if (commandData.arguments.size() > 0) {
        throw SemanticError("arguments not expected", commandData.arguments[0].position);
    }
    if (commandData.options.size() > 0) {
        throw SemanticError("option not expected", commandData.options[0].position);
    }
}

void DateCommand::execute(std::istream& input, std::ostream& output, CommandLineInterpreter& cli) {
    std::time_t time = std::time(0);
    std::tm* now = std::localtime(&time);
    output << now->tm_mday << '.' << now->tm_mon + 1 << '.' << now->tm_year + 1900 << '.';
}