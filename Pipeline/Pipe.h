#ifndef Pipe_h
#define Pipe_h

#include <string>
#include <vector>

#include "../Commands/Command.h"
#include "../DataTypes/DataTypes.h"

class Pipe {

public:
    // Deletes all commands. It is important to insert every created
    // command into commands vector so it doesn't stay undeleted when pipe is deleted
    ~Pipe();
    
    // Calls tokenize and fills tokens vector based on it (1 vector between each |)
    // Creates all the commands and places them in commands vector
    // Takes care of syntax and some semantic errors. If line isn't parsable, then an error occured (could be caught in tokenize)
    void parse(const std::string& line);

    // Only the first command in pipeline can take input as an argument, and every other command has to accept input
    // Only the last command can redirect output to the file, and every other command has to produce output
    // Validate takes care of this
    void validate();

    // Executes all the commands, and takes care of input and output locations
    void execute(std::istream& defaultInput, std::ostream& defaultOutput, CommandLineInterpreter& cli);
    
private:
    // Splits whole command line in tokens
    std::vector<Token> tokenize(const std::string& line);
    
    // Creates concrete command, calls parse and validate for it,
    // which makes it ready for execution (if no errors occured)
    Command* createCommand(const std::vector<Token>& tokens);

    std::vector<std::vector<Token>> tokens;
    std::vector<Command*> commands;

};

#endif