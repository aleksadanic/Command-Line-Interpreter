#ifndef TouchCommand_h
#define TouchCommand_h

#include "Command.h"

class TouchCommand : public Command {

public:
    void validate() override;
    void execute(std::istream& input, std::ostream& output, CommandLineInterpreter& cli) override;
    bool acceptsInput() const override { return true; };
    bool producesOutput() const override { return false; }
    static inline std::string name() { return "touch"; }

};

#endif