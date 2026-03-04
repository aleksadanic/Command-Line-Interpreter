#ifndef Command_h
#define Command_h

#include <vector>
#include <string>
#include <sstream>

#include "../CommandLineInterpreter/CommandLineInterpreter.h"
#include "../DataTypes/DataTypes.h"

class Command {

public:

    virtual ~Command() { }

    // Creates and returns an instance of concrete command based on its name
    static Command* factory(const std::string& str);

    // If received argument represents a filename, that file is being opened and its content is
    // placed back into arg.content. arg.type is changed accordingly
    static void processArgument(Argument& arg);

    // Extracts input and output redirections (if they exist), and throws SemanticException if
    // redirections are placed wrongly. Fills commandData.redirIN and commandData.redirOUT accordingly
    virtual void extractRedirections(const std::vector<Token>& tokens);

    // Extracts all the relevant data from tokens and fills according commandData fields
    // After this function, command doesn't need tokens anymore - all the information is in commandData
    // Throws exception if tokens vector is invalid
    // Universal for almost every command
    virtual void parse(const std::vector<Token>& tokens);

    // Every concrete command checks if arguments and options are suitable
    // If not, throws an exception
    virtual void validate() = 0;

    // Calls processArgument for its arguments if needed
    virtual void prepare() { }

    // Executes concrete command
    virtual void execute(std::istream& input, std::ostream& output, CommandLineInterpreter& cli) = 0;

    const CommandData& getCommandData() const { return commandData; }

    // Returns true if concrete command can accept input
    virtual bool acceptsInput() const = 0;

    // Returns true if concrete command produces output
    virtual bool producesOutput() const = 0;


protected:
    CommandData commandData;

};

#endif