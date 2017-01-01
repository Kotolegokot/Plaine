#pragma once

#include <iostream>
#include <string>
#include <list>
#include <exception>

#include "ParseError.hpp"
#include "Lexeme.hpp"

class ConsoleInterface {
    void parse_string(const std::string &str);
    void execute_cmd(const std::string &cmd, const std::list<Lexeme> &args);

public:
    void run();
};
