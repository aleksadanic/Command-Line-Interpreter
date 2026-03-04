#ifndef SyntaxError_h
#define SyntaxError_h

#include "Exception.h"

class SyntaxError : public Exception {
public:
    SyntaxError(std::string _message, int _pos) : Exception(_message, _pos) {
        message = "SyntaxError: " + message;
    }
};

#endif