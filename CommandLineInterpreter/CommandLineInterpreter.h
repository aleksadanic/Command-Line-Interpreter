#ifndef CommandLineInterpreter_h
#define CommandLineInterpreter_h

#include <string>

class CommandLineInterpreter {

public:
    // Reads command lines from in, and writes results to out by default
    void readCommands(std::istream& in, std::ostream& out);

    inline std::string getPrompt() const;
    inline void setPrompt(const std::string& str);

private:
    std::string prompt = "$";

};

inline std::string CommandLineInterpreter::getPrompt() const {
    return prompt;
}

inline void CommandLineInterpreter::setPrompt(const std::string& str) {
    prompt = str;
}

#endif