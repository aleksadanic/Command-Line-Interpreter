#include "RmCommand.h"

#include "../Exceptions/AllExceptions.h"
#include "../FileManager/FileManager.h"

void RmCommand::validate() {
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

void RmCommand::execute(std::istream& input, std::ostream& output, CommandLineInterpreter& cli) {
    FileManager::removeFile(commandData.arguments[0].content);
}