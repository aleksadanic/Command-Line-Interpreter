#ifndef SemanticError_h
#define SemanticError_h

#include "Exception.h"

class SemanticError : public Exception {
public:
    SemanticError(std::string _message, int _pos) : Exception(_message, _pos) {
        message = "SemanticError: " + message;
    }
};

#endif