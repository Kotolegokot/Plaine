#include "ConsoleInterface.hpp"

static std::list<Lexeme> lexer(const std::string &str);

void ConsoleInterface::parse_string(const std::string &str)
{
    auto lexemes = lexer(str);
    if (lexemes.empty())
        return;

    if (lexemes.front().type() != Lexeme::SYMBOL)
        throw ParseError("invalid command type: " + lexemes.front().show_type() +
                         " (" + lexemes.front().show() + ")");

    std::string cmd = lexemes.front().from_symbol();
    lexemes.pop_front();

    execute_cmd(cmd, lexemes);
}

void ConsoleInterface::execute_cmd(const std::string &cmd, const std::list<Lexeme> &args)
{
    // checking cmds

    // else
    //     ParseError("undefined command: '" + cmd + "'");
}

void ConsoleInterface::run()
{
    while (!std::cin.fail()) {
        std::string s;
        std::getline(std::cin, s);
        parse_string(s);
    }

}

static std::list<Lexeme> lexer(const std::string &str)
{
    enum { DEFAULT, SYMBOL, STRING, INT, FLOAT } state = DEFAULT;
    std::list<Lexeme> acc;
    std::string str_acc;

    std::size_t i = 0;
    while (i <= str.size()) {
        int c = (i == str.size()) ? -1 : str[i];

        switch (state) {
        case DEFAULT:
            if (isspace(c) || c == -1)
                i++;
            else if (isalpha(c) || c == '_')
                state = SYMBOL;
            else if (isdigit(c) || c == '-' || c == '+') {
                state = INT;
                str_acc.push_back(c);
                i++;
            } else if (c == '"') {
                state = STRING;
                i++;
            } else
                throw ParseError("unexpected character: '" + std::string(c, 1) + "'");
            break;
        case SYMBOL:
            if (isalnum(c) || c == '_') {
                str_acc.push_back(c);
                i++;
            } else {
                acc.push_back(Lexeme::create_symbol(str_acc));
                str_acc.clear();
                state = DEFAULT;
            }
            break;
        case STRING:
            if (c == -1)
                throw ParseError("'\"' expected, end of line found");
            else if (c != '"') {
                str_acc.push_back(c);
                i++;
            } else {
                acc.push_back(Lexeme::create_string(str_acc));
                str_acc.clear();
                i++;
                state = DEFAULT;
            }
            break;
        case INT:
            if (isdigit(c)) {
                str_acc.push_back(c);
                i++;
            } else if (c == '.') {
                str_acc.push_back(c);
                i++;
                state = FLOAT;
            } else {
                acc.push_back(Lexeme::create_int(std::stoi(str_acc)));
                str_acc.clear();
                state = DEFAULT;
            }
            break;
        case FLOAT:
            if (isdigit(c)) {
                str_acc.push_back(c);
                i++;
            } else if (c == '.')
                throw ParseError("second '.' in a number");
            else {
                acc.push_back(Lexeme::create_float(std::stof(str_acc)));
                str_acc.clear();
                state = DEFAULT;
            }
            break;
        }
    }

    return acc;
}
