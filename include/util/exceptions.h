#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>

class initialization_error : public std::exception {
public:
    const char *what() const noexcept override
    {
        return "failed to initialize device";
    }
};

class repeated_initialization : public std::exception {
public:
    const char *what() const noexcept override
    {
        return "repeated device initialization";
    }
};

#endif // EXCEPTIONS_H
