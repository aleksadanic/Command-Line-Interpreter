#include "TrCommand.h"

#include "../Exceptions/AllExceptions.h"

void TrCommand::parse(const std::vector<Token>& tokens) {
    extractRedirections(tokens);
    int n = tokens.size();
    while (n > 0 && (tokens[n - 1].type == TokenType::REDIRECT_IN ||
                     tokens[n - 1].type == TokenType::REDIRECT_OUT_APPEND ||
                     tokens[n - 1].type == TokenType::REDIRECT_OUT_TRUNCATE)) {
        n--;
    }
    int x = 2;
    if (1 >= n) {
        throw SemanticError("option expected", -1);
    }
    if (tokens[1].content != "-") {
        if (tokens[1].type == TokenType::NORMAL) {
            commandData.inputSpecified = true;
            commandData.arguments.push_back(Argument(ArgumentType::FILENAME, tokens[1].content, tokens[1].position));
        } else if (tokens[1].type == TokenType::QUOTED) {
            commandData.inputSpecified = true;
            commandData.arguments.push_back(Argument(ArgumentType::LITERAL, tokens[1].content, tokens[1].position));
        } else {
            throw Exception("error occurred", -1);
        }
    } else {
        x--;
    }
    if (x >= n) {
        throw SemanticError("option expected", -1);
    }
    if (tokens[x].content != "-") {
        throw SemanticError("option expected", tokens[x].position);
    }
    if (x + 1 >= n) {
        throw SemanticError("option wrongly specified", tokens[x].position + 1);
    }
    if (tokens[x + 1].type != TokenType::QUOTED) {
        throw SemanticError("expected quoted option", tokens[x].position + 1);
    }
    commandData.options.push_back(Option(tokens[x + 1].content, tokens[x + 1].position));
    if (x + 2 >= n) {
        commandData.arguments.push_back(Argument(ArgumentType::LITERAL, "", 0));
    } else {
        if (tokens[x + 2].type != TokenType::QUOTED) {
            throw SemanticError("expected quoted [with] argument", tokens[x + 2].position);
        }
        commandData.arguments.push_back(Argument(ArgumentType::LITERAL, tokens[x + 2].content, tokens[x + 1].position));
        if (x + 3 < n) {
            throw SemanticError("didn't expect anything after [with] argument", tokens[x + 3].position);
        }
    }
}

void TrCommand::validate() {
    if (commandData.options[0].content.size() == 0) {
        throw SemanticError("option can't be empty", commandData.options[0].position);
    }
}

void TrCommand::prepare() {
    if (commandData.arguments.size() != 1) {
        processArgument(commandData.arguments[0]);
    }
}

void TrCommand::execute(std::istream& input, std::ostream& output, CommandLineInterpreter& cli) {
    std::string data = "";
    if (commandData.arguments.size() == 1) {
        char ch;
        while (input.get(ch)) {
            data += ch;
        }
    } else {
        data = commandData.arguments[0].content;
    }
    std::string& replaceWhat = commandData.options[0].content;
    std::string& replaceWith = commandData.arguments.back().content;
    for (int i = 0; i < (int)data.size(); i++) {
        if (data.substr(i, replaceWhat.size()) == replaceWhat) {
            output << replaceWith;
            i += replaceWhat.size() - 1;
        } else {
            output << data[i];
        }
    }
}