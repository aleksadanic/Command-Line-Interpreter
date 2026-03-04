#ifndef Exception_h
#define Exception_h

#include <string>
#include <exception>

class Exception : public std::exception {
public:
    Exception(std::string _message, int _pos) : message(_message), pos(_pos) { }
    virtual const char * what() const noexcept override { return message.c_str(); }
    int getPos() const { return pos; }
protected:
    std::string message;
    int pos = -1;
};

#endif