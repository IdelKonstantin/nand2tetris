#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>
#include <fstream>

class Parser {

private:

    const char m_destCompDelimeter = '=';
    const char m_compJumpDelimeter = ';';

    void trimString(std::string& codeLine) const;
    std::string checkForComments(const std::string& codeLine) const;

    std::vector<std::string> m_asmBuff{};


public:

    enum instruction_t {

        A_INSTRUCTION,
        C_INSTRUCTION,
        L_INSTRUCTION
    };

    Parser();

    std::vector<std::string> trimAndSkipComments(std::ifstream& asmFile);

    instruction_t instructionType(const std::string& codeLine) const;
    std::string symbol(const std::string& codeLine) const;
    std::string dest(const std::string& codeLine) const;
    std::string comp(const std::string& codeLine) const;
    std::string jump(const std::string& codeLine) const;
};

#endif // PARSER_H
