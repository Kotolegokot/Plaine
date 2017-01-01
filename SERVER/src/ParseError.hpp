#pragma once

class ParseError : public std::exception
{
    std::string m_what;
public:
    ParseError(const std::string &w) : m_what(w) {}
    virtual ~ParseError() {}
    const char *what() const noexcept override { return m_what.c_str(); }
};
