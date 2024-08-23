#ifndef CODE_H
#define CODE_H

#include <string>
#include <sstream>
#include <map>

#include "parser.h"

class CodeWriter {

private:

    std::stringstream m_ss;
    const std::map<std::string, std::string> m_segNicknames {

        {"local", "LCL"},
        {"argument", "ARG"},
        {"this", "THIS"},
        {"that", "THAT"}
    };

    uint16_t m_eqCounter{0};
    uint16_t m_gtCounter{0};
    uint16_t m_ltCounter{0};

public:
    CodeWriter();

    std::string writeArithmetic(const std::string& opName, const std::string& currentWorkingModule);
    std::string writePushPop(Parser::command_t commandType, const std::string& segment, const std::string& index, const std::string& currentWorkingModule);
};

#endif // CODE_H
