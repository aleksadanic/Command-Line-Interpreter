#include "TruncateCommand.h"

#include "../Exceptions/AllExceptions.h"
#include "../FileManager/FileManager.h"

void TruncateCommand::validate() {
    if (commandData.options.size() > 0) {
        throw SemanticError("option not expected", commandData.options[0].position);
    }
    if (commandData.arguments.size() == 0) {
        throw SemanticError("filename not specified", -1);
    }
    if (commandData.arguments.size() > 1) {
        throw SemanticError("too many arguments", commandData.arguments[1].position);
    }
    if (commandData.arguments[0].type != ArgumentType::FILENAME) {
        throw SemanticError("filename wrongly specified", commandData.arguments[0].position);
    }
}

void TruncateCommand::execute(std::istream& input, std::ostream& output, CommandLineInterpreter& cli) {
    FileManager::truncateFile(commandData.arguments[0].content);
}