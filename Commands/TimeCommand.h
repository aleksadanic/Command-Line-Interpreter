#ifndef TimeCommand_h
#define TimeCommand_h

#include "Command.h"

class TimeCommand : public Command {

public:
    void validate() override;
    void execute(std::istream& input, std::ostream& output, CommandLineInterpreter& cli) override;
    bool acceptsInput() const override { return false; };
    bool producesOutput() const override { return true; }
    static inline std::string name() { return "time"; }

};

#endif