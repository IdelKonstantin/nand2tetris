#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>
#include <map>
#include <fstream>

class Parser {

private:

    void trimTrailingSpaces(std::string& line) const;
    std::string checkForComments(const std::string& codeLine) const;
    std::vector<std::string> m_vmBuff{};

    void trimReturn(std::string& argRaw) const;
    void trimBeginSpaces(std::string& codeLine) const;

public:

    enum command_t {

        C_ARITHMETIC,
        C_PUSH,
        C_POP,
        C_LABEL,
        C_GOTO,
        C_IF,
        C_FUNCTION,
        C_RETURN,
        C_CALL
    };

    const std::map<std::string, command_t> m_commandStamps {

        {"push", C_PUSH},
        {"pop", C_POP},
        {"function", C_FUNCTION},
        {"return", C_RETURN},
        {"call", C_CALL},
        {"label", C_LABEL},
        {"goto", C_GOTO},
        {"if-goto", C_IF}
    };

    Parser();

    std::vector<std::string> trimAndSkipComments(std::ifstream& vmFile);

    command_t commandType(const std::string& codeLine) const;
    std::string arg1(const std::string& codeLine) const;
    std::string arg2(const std::string& codeLine) const;
};

#endif // PARSER_H
