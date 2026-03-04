#ifndef DataTypes_h
#define DataTypes_h

#include <string>
#include <vector>

enum class TokenType { NORMAL, QUOTED, REDIRECT_IN, REDIRECT_OUT_TRUNCATE, REDIRECT_OUT_APPEND, PIPE };
struct Token {
    TokenType type;
    std::string content;
    int position = -1;
    
    Token(TokenType _type, std::string _content, int _position) :
        type(_type), content(_content), position(_position) { }
    Token() { }
};


enum class ArgumentType { LITERAL, FILENAME };
enum class RedirectedInputType { NONE, FILE };
enum class RedirectedOutputType { NONE, TRUNCATE, APPEND };

struct Argument {
    ArgumentType type;
    std::string content;
    int position = -1;

    Argument(ArgumentType _type, std::string _content, int _position) :
        type(_type), content(_content), position(_position) { }
    Argument() { }
};

struct Option {
    std::string content;
    int position = -1;

    Option(std::string _content, int _position) :
        content(_content), position(_position) { }
    Option() { }
};

struct RedirectedInput {
    RedirectedInputType type = RedirectedInputType::NONE;
    std::string content;
    int position = -1;

    RedirectedInput(RedirectedInputType _type, std::string _content, int _position) :
        type(_type), content(_content), position(_position) { }
    RedirectedInput() { }
};

struct RedirectedOutput {
    RedirectedOutputType type = RedirectedOutputType::NONE;
    std::string content;
    int position = -1;

    RedirectedOutput(RedirectedOutputType _type, std::string _content, int _position) :
        type(_type), content(_content), position(_position) { }
    RedirectedOutput() { }
};

struct CommandData {
    bool inputSpecified = false;
    std::vector<Argument> arguments;
    std::vector<Option> options;
    RedirectedInput redirIN;
    RedirectedOutput redirOUT;
};

#endif