#ifndef TrCommand_h
#define TrCommand_h

#include "Command.h"

class TrCommand : public Command {

public:
    // TrCommand has special parse because argument comes before option
    // in a command line, contrary to the majority of commands
    void parse(const std::vector<Token>& tokens) override;
    void validate() override;
    void prepare() override;
    void execute(std::istream& input, std::ostream& output, CommandLineInterpreter& cli) override;
    bool acceptsInput() const override { return true; };
    bool producesOutput() const override { return true; }
    static inline std::string name() { return "tr"; };

};

#endif