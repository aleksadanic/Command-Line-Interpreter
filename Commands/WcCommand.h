#ifndef WcCommand_h
#define WcCommand_h

#include "Command.h"

class WcCommand : public Command {

public:
    void validate() override;
    void prepare() override;
    void execute(std::istream& input, std::ostream& output, CommandLineInterpreter& cli) override;
    bool acceptsInput() const override { return true; };
    bool producesOutput() const override { return true; }
    static inline std::string name() { return "wc"; }

};

#endif