#ifndef RmCommand_h
#define RmCommand_h

#include "Command.h"

class RmCommand : public Command {

public:
    void validate() override;
    void execute(std::istream& input, std::ostream& output, CommandLineInterpreter& cli) override;
    bool acceptsInput() const override { return true; };
    bool producesOutput() const override { return false; }
    static inline std::string name() { return "rm"; };

};

#endif