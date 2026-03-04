#include "AllCommands.h"
#include "../Util/Util.h"
#include "../Exceptions/AllExceptions.h"
#include "../FileManager/FileManager.h"

Command* Command::factory(const std::string& str) {
    Command* command = nullptr;
    if (str == EchoCommand::name()) {
        command = new EchoCommand();
    } else if (str == TimeCommand::name()) {
        command = new TimeCommand();
    } else if (str == DateCommand::name()) {
        command = new DateCommand();
    } else if (str == TouchCommand::name()) {
        command = new TouchCommand();
    } else if (str == TruncateCommand::name()) {
        command = new TruncateCommand();
    } else if (str == RmCommand::name()) {
        command = new RmCommand();
    } else if (str == WcCommand::name()) {
        command = new WcCommand();
    } else if (str == PromptCommand::name()) {
        command = new PromptCommand();
    } else if (str == BatchCommand::name()) {
        command = new BatchCommand();
    } else if (str == HeadCommand::name()) {
        command = new HeadCommand();
    } else if (str == TrCommand::name()) {
        command = new TrCommand();
    }
    return command;
}

void Command::processArgument(Argument& arg) {
    if (arg.type == ArgumentType::FILENAME) {
        std::istream* file = FileManager::openInputFile(arg.content);
        arg.content.clear();
        char ch;
        while ((*file).get(ch)) {
            arg.content += ch;
        }
        arg.type = ArgumentType::LITERAL;
        delete file;
    }
}

void Command::extractRedirections(const std::vector<Token>& tokens) {
    int x = -1, y = -1;
    for (int i = tokens.size() - 1; i >= 0; i--) {
        if (tokens[i].type == TokenType::REDIRECT_IN) {
            x = i;
        } else if (tokens[i].type == TokenType::REDIRECT_OUT_TRUNCATE || tokens[i].type == TokenType::REDIRECT_OUT_APPEND) {
            y = i;
        }
    }
    
    if (x == -1 && y == -1) {
        return;
    }

    if (x == -1) {
        if (y != (int)tokens.size() - 1) {
            throw SemanticError("didn't expect anything after output redirection", tokens[y + 1].position);
        }
    }
    if (y == -1) {
        if (x != (int)tokens.size() - 1) {
            throw SemanticError("didn't expect anything after input redirection", tokens[x + 1].position);
        }
    }
    if (x != -1 && y != -1) {
        if ((x != (int)tokens.size() - 2 || y != (int)tokens.size() - 1) && (x != (int)tokens.size() - 1 || y != (int)tokens.size() - 2)) {
            if (x < y) {
                throw SemanticError("didn't expect anything after input redirection", tokens[x + 1].position);
            } else {
                throw SemanticError("didn't expect anything after output redirection", tokens[y + 1].position);
            }
        }
    }

    if (x != -1) {
        commandData.redirIN = RedirectedInput(RedirectedInputType::FILE, tokens[x].content, tokens[x].position);
    }

    if (y != -1) {
        if (tokens[y].type == TokenType::REDIRECT_OUT_TRUNCATE) {
            commandData.redirOUT = RedirectedOutput(RedirectedOutputType::TRUNCATE, tokens[y].content, tokens[y].position);
        } else if (tokens[y].type == TokenType::REDIRECT_OUT_APPEND) {
            commandData.redirOUT = RedirectedOutput(RedirectedOutputType::APPEND, tokens[y].content, tokens[y].position);
        } else {
            throw Exception("error occurred", -1);
        }
    }
}

void Command::parse(const std::vector<Token>& tokens) {
    extractRedirections(tokens);
    int n = tokens.size();
    while (n > 0 && (tokens[n - 1].type == TokenType::REDIRECT_IN ||
                     tokens[n - 1].type == TokenType::REDIRECT_OUT_APPEND ||
                     tokens[n - 1].type == TokenType::REDIRECT_OUT_TRUNCATE)) {
        n--;
    }
    bool seenArgumentBefore = false;
    for (int i = 1; i < n; i++) {
        if (tokens[i].type == TokenType::NORMAL && tokens[i].content.size() > 0 && tokens[i].content[0] == '-') {
            if (seenArgumentBefore) {
                throw SemanticError("option not expected", tokens[i].position);
            }
            if (tokens[i].content.size() > 1) {
                commandData.options.push_back(Option(tokens[i].content.substr(1), tokens[i].position));
            } else if (i + 1 < n && tokens[i].position + 1 == tokens[i + 1].position && tokens[i + 1].type == TokenType::QUOTED) {
                commandData.options.push_back(Option(tokens[i + 1].content, tokens[i].position));
                i++;
            } else {
                throw SemanticError("invalid option", tokens[i].position + 1);
            }
        } else if (tokens[i].type == TokenType::NORMAL) {
            seenArgumentBefore = true;
            commandData.inputSpecified = true;
            commandData.arguments.push_back(Argument(ArgumentType::FILENAME, tokens[i].content, tokens[i].position));
        } else if (tokens[i].type == TokenType::QUOTED) {
            seenArgumentBefore = true;
            commandData.inputSpecified = true;
            commandData.arguments.push_back(Argument(ArgumentType::LITERAL, tokens[i].content, tokens[i].position));
        } else {
            throw Exception("error occurred", -1);
        }
    }
}