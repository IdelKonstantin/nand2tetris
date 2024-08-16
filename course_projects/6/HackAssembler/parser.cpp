#include "parser.h"

#include <algorithm>

Parser::Parser() {}

std::vector<std::string> Parser::trimAndSkipComments(std::ifstream& asmFile) {

    std::string line;

    while(std::getline(asmFile, line)) {

        trimString(line);
        line = checkForComments(line);

        if(!line.empty()) {
            m_asmBuff.push_back(line);
        }
    }

    asmFile.close();
    return m_asmBuff;
}

void Parser::trimString(std::string& codeLine) const {
    codeLine.erase(std::remove_if(codeLine.begin(), codeLine.end(), ::isspace), codeLine.end());
}

std::string Parser::checkForComments(const std::string& codeLine) const {
    if(codeLine[0] == '/' && codeLine[1] == '/') {
        return std::string{};
    }

    auto pos = codeLine.find("//");

    if(pos != std::string::npos) {
        return codeLine.substr(0, pos);
    }
    else {
        return codeLine;
    }
}

Parser::instruction_t Parser::instructionType(const std::string& codeLine) const {

    if(codeLine[0] == '@') {
        return A_INSTRUCTION;
    }
    else if(*codeLine.begin() == '(' && *(codeLine.end() - 1) == ')') {
        return L_INSTRUCTION;
    }
    else {
        return C_INSTRUCTION;
    }
}

std::string Parser::symbol(const std::string& codeLine) const {
    if(codeLine[0] == '@') {
        return codeLine.substr(1, codeLine.length());
    }
    else if(*codeLine.begin() == '(' && *(codeLine.end() - 1) == ')') {
        return codeLine.substr(1, codeLine.length() - 2);
    }
}

std::string Parser::dest(const std::string& codeLine) const {

    auto posDestCompDelimeter = codeLine.find(m_destCompDelimeter);

    if(posDestCompDelimeter == std::string::npos) {
        return std::string{"null"};
    }

    return codeLine.substr(0, posDestCompDelimeter);
}

std::string Parser::comp(const std::string& codeLine) const {

    auto posCompJumpDelimeter = codeLine.find(m_compJumpDelimeter);
    auto posDestCompDelimeter = codeLine.find(m_destCompDelimeter);

    if(posCompJumpDelimeter == std::string::npos) {
        return codeLine.substr(posDestCompDelimeter + 1, codeLine.length());
    }
    if(posDestCompDelimeter == std::string::npos) {
        posDestCompDelimeter = -1;
    }

    return codeLine.substr(posDestCompDelimeter + 1, posCompJumpDelimeter - posDestCompDelimeter - 1);
}

std::string Parser::jump(const std::string& codeLine) const {

    auto posCompJumpDelimeter = codeLine.find(m_compJumpDelimeter);

    if(posCompJumpDelimeter == std::string::npos) {
        return std::string{"null"};
    }

    return codeLine.substr(posCompJumpDelimeter + 1, codeLine.length());
}
