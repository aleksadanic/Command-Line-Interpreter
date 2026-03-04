#include "BatchCommand.h"

#include <memory>

#include "../Exceptions/AllExceptions.h"
#include "../FileManager/FileManager.h"

void BatchCommand::validate() {
    if (commandData.options.size() > 0) {
        throw SemanticError("option not expected", commandData.options[0].position);
    }
    if (commandData.arguments.size() == 0) {
        throw SemanticError("filename not specified", -1);
    }
    if (commandData.arguments.size() > 1 || commandData.arguments[0].type != ArgumentType::FILENAME) {
        throw SemanticError("filename wrongly specified", -1);
    }
}

void BatchCommand::execute(std::istream& input, std::ostream& output, CommandLineInterpreter& cli) {
    std::unique_ptr<std::istream> newInput(FileManager::openInputFile(commandData.arguments[0].content));
    cli.readCommands(*newInput, output);
}