#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <stdexcept>
#include <string>

class GenexException : public std::runtime_error
{
public:
  GenexException(const char* msg) : std::runtime_error(msg) {}
  GenexException(std::string msg) : std::runtime_error(msg.c_str()) {}
};


#endif // EXCEPTION_H
