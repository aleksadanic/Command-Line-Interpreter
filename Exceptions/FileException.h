#ifndef FileException_h
#define FileException_h

#include "Exception.h"

class FileException : public Exception {
public:
    FileException(std::string _message, int _pos) : Exception(_message, _pos) {
        message = "FileException: " + message;
    }
};

#endif