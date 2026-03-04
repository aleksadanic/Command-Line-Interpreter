#include "Pipe.h"

#include <memory>

#include "../Commands/AllCommands.h"
#include "../Exceptions/AllExceptions.h"
#include "../DataTypes/DataTypes.h"
#include "../FileManager/FileManager.h"

Pipe::~Pipe() {
    for (Command* command : commands) {
        delete command;
    }
}

std::vector<Token> Pipe::tokenize(const std::string& line) {
    auto isimportant = [&](const char& c) {
        return c == '|' || c == '<' || c == '>' || c == '"';
    };

    std::vector<Token> tokens;

    bool inQuotes = false;
    int leftQuote = -1;

    for (int i = 0; i < (int)line.size(); i++) {
        if (inQuotes) {
            if (line[i] == '"') {
                tokens.push_back(Token(TokenType::QUOTED, line.substr(leftQuote + 1, i - leftQuote - 1), leftQuote));
                inQuotes = false;
            }
            continue;
        }

        if (line[i] == '"') {
            leftQuote = i;
            inQuotes = true;
        } else if (line[i] == '|') {
            tokens.push_back(Token(TokenType::PIPE, "", i));
        } else if (line[i] == '<') {
            int fst = i + 1;
            for (; fst < (int)line.size(); fst++) {
                if (!std::isspace((unsigned char)line[fst])) {
                    break;
                }
            }
            int j = fst;
            for (; j < (int)line.size(); j++) {
                if (std::isspace((unsigned char)line[j]) || isimportant(line[j])) {
                    break;
                }
            }
            if (j - fst <= 0) {
                throw SemanticError("redirection input file not specified", i);
            }
            tokens.push_back(Token(TokenType::REDIRECT_IN, line.substr(fst, j - fst), fst));
            i = j - 1;
        } else if (line[i] == '>') {
            int fst = i + 1;
            bool append = false;
            if (i + 1 < (int)line.size() && line[i + 1] == '>') {
                fst++;
                append = true;
            }
            for (; fst < (int)line.size(); fst++) {
                if (!std::isspace((unsigned char)line[fst])) {
                    break;
                }
            }
            int j = fst;
            for (; j < (int)line.size(); j++) {
                if (std::isspace((unsigned char)line[j]) || isimportant(line[j])) {
                    break;
                }
            }
            if (j - fst <= 0) {
                throw SemanticError("redirection output file not specified", i);
            }
            tokens.push_back(Token((append ? TokenType::REDIRECT_OUT_APPEND : TokenType::REDIRECT_OUT_TRUNCATE), line.substr(fst, j - fst), fst));
            i = j - 1;
        } else if (!std::isspace((unsigned char)line[i])) {
            int j = i + 1;
            for (; j < (int)line.size(); j++) {
                if (std::isspace((unsigned char)line[j]) || isimportant(line[j])) {
                    break;
                }
            }
            tokens.push_back(Token(TokenType::NORMAL, line.substr(i, j - i), i));
            i = j - 1;
        }
    }

    if (inQuotes) {
        throw SyntaxError("unpaired quote", leftQuote);
    }

    return tokens;
}

Command* Pipe::createCommand(const std::vector<Token>& tokens) {
    if (tokens[0].type != TokenType::NORMAL) {
        throw SyntaxError("unknown command", tokens[0].position);
    }
    Command* command = Command::factory(tokens[0].content);
    if (!command) {
        throw SyntaxError("unknown command - " + tokens[0].content, tokens[0].position);
    }
    try {
        command->parse(tokens);
        command->validate();
        return command;
    } catch (...) {
        delete command;
        throw;
    }
}

void Pipe::parse(const std::string& line) {
    std::vector<Token> allTokens = tokenize(line);
    tokens = {{}};
    int lastPipe = -1;
    for (const Token& token : allTokens) {
        if (token.type == TokenType::PIPE) {
            if (tokens.back().empty()) {
                throw SemanticError("empty command", token.position - 1);
            }
            commands.push_back(createCommand(tokens.back()));
            tokens.push_back({});
            lastPipe = token.position;
        } else {
            tokens.back().push_back(token);
        }
    }
    if (tokens.back().empty()) {
        throw SemanticError("empty command", lastPipe + 1);
    }
    commands.push_back(createCommand(tokens.back()));
}

void Pipe::validate() {
    for (int i = 0; i < (int)commands.size(); i++) {
        if (i > 0) {
            if (!commands[i]->acceptsInput()) {
                throw SemanticError("command doesn't accept input", tokens[i][0].position);
            }
            if (commands[i]->getCommandData().inputSpecified) {
                throw SemanticError("didn't expect specified input", tokens[i][0].position);
            }
            if (commands[i]->getCommandData().redirIN.type != RedirectedInputType::NONE) {
                throw SemanticError("didn't expect input redirection", tokens[i][0].position);
            }
        } else {
            if (commands[i]->getCommandData().inputSpecified && commands[i]->getCommandData().redirIN.type != RedirectedInputType::NONE) {
                throw SemanticError("input location ambiguous", -1);
            }
        }
        if (i < (int)commands.size() - 1) {
            if (!commands[i]->producesOutput()) {
                throw SemanticError("command doesn't produce output", tokens[i][0].position);
            }
            if (commands[i]->getCommandData().redirOUT.type != RedirectedOutputType::NONE) {
                throw SemanticError("didn't expect output redirection", tokens[i][0].position);
            }
        }
        commands[i]->prepare();
    }
}

void Pipe::execute(std::istream& defaultInput, std::ostream& defaultOutput, CommandLineInterpreter& cli) {
    if (commands.size() == 0) {
        return;
    }

    std::vector<std::stringstream> pipes(commands.size() - 1);
    
    for (int i = 0; i < (int)commands.size(); i++) {
        std::unique_ptr<std::istream> inputOwner;
        std::istream* input = nullptr;

        std::unique_ptr<std::ostream> outputOwner;
        std::ostream* output = nullptr;

        // input
        if (i == 0) {
            if (commands[i]->getCommandData().redirIN.type != RedirectedInputType::NONE) {
                inputOwner.reset(FileManager::openInputFile(commands[i]->getCommandData().redirIN.content));
                input = inputOwner.get();
            } else {
                input = &defaultInput;
            }
        } else {
            input = &pipes[i - 1];
        }

        // output
        if (i == (int)commands.size() - 1) {
            if (commands[i]->getCommandData().redirOUT.type != RedirectedOutputType::NONE) {
                if (commands[i]->getCommandData().redirOUT.type == RedirectedOutputType::APPEND) {
                    outputOwner.reset(FileManager::openOutputFile(commands[i]->getCommandData().redirOUT.content, true));
                    output = outputOwner.get();
                } else if (commands[i]->getCommandData().redirOUT.type == RedirectedOutputType::TRUNCATE) {
                    outputOwner.reset(FileManager::openOutputFile(commands[i]->getCommandData().redirOUT.content, false));
                    output = outputOwner.get();
                } else {
                    throw Exception("error occurred", -1);
                }
            } else {
                output = &defaultOutput;
            }
        } else {
            output = &pipes[i];
        }

        commands[i]->execute(*input, *output, cli);
    }
    if (commands.back()->producesOutput() && commands.back()->getCommandData().redirOUT.type == RedirectedOutputType::NONE) {
        defaultOutput << '\n';
    }
}