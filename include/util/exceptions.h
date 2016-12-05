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

class invalid_audiosource : public std::exception {
    unsigned int m_source;
    std::string m_what;
public:
    invalid_audiosource(unsigned int source) :
        m_source(source)
    {
        m_what = "invalid audio source (";
        m_what += m_source;
        m_what += ")";
    }

    const char *what() const noexcept override
    {
        return m_what.c_str();
    }
};

#endif // EXCEPTIONS_H
