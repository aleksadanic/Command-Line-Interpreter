#ifndef DateCommand_h
#define DateCommand_h

#include "Command.h"

class DateCommand : public Command {

public:
    void validate() override;
    void execute(std::istream& input, std::ostream& output, CommandLineInterpreter& cli) override;
    bool acceptsInput() const override { return false; };
    bool producesOutput() const override { return true; }
    static inline std::string name() { return "date"; };

};

#endif