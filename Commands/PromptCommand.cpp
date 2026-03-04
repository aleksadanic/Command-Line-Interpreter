#include "PromptCommand.h"

#include "../Exceptions/AllExceptions.h"
#include "../FileManager/FileManager.h"

void PromptCommand::validate() {
    if (commandData.options.size() > 0) {
        throw SemanticError("option not expected", commandData.options[0].position);
    }
    if (commandData.arguments.size() == 0) {
        throw SemanticError("prompt not specified", -1);
    }
    if (commandData.arguments[0].type != ArgumentType::LITERAL) {
        throw SemanticError("expected quotes", commandData.arguments[0].position);
    }
    if (commandData.arguments.size() > 1) {
        throw SemanticError("too many arguments", commandData.arguments[1].position);
    }
}

void PromptCommand::execute(std::istream& input, std::ostream& output, CommandLineInterpreter& cli) {
    cli.setPrompt(commandData.arguments[0].content);
}