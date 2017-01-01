#pragma once

#include <string>
#include <iostream>

struct Lexeme {
    enum Type { SYMBOL, STRING, INT, FLOAT };

    Type type() const { return m_type; }

    std::string from_symbol() const { return m_str; }
    std::string from_string() const { return m_str; }
    int from_int() const { return m_int; }
    float from_float() const { return m_float; }

    static Lexeme create_symbol(const std::string &str)
    {
        Lexeme l;
        l.m_type = SYMBOL;
        l.m_str = str;
        return l;
    }

    static Lexeme create_string(const std::string &str)
    {
        Lexeme l;
        l.m_type = STRING;
        l.m_str = str;
        return l;
    }

    static Lexeme create_int(int i)
    {
        Lexeme l;
        l.m_type = INT;
        l.m_int = i;
        return l;
    }

    static Lexeme create_float(float f)
    {
        Lexeme l;
        l.m_type = FLOAT;
        l.m_float = f;
        return l;
    }

    std::string show_type() const
    {
        switch (m_type) {
        case SYMBOL:
            return "symbol";
        case STRING:
            return "string";
        case INT:
            return "int";
        case FLOAT:
            return "float";
        default:
            return "";
        }
    }

    std::string show() const
    {
        switch (m_type) {
        case SYMBOL:
            return m_str;
        case STRING:
            return "\"" + m_str + "\"";
        case INT:
            return std::to_string(m_int);
        case FLOAT:
            return std::to_string(m_float);
        default:
            return "";
        }
    }

    friend std::ostream &operator <<(std::ostream &out, const Lexeme &l)
    {
        return out << l.show();
    }

private:
    Type m_type;

    std::string m_str;
    int m_int;
    float m_float;

    Lexeme() {}
};
