#include "parser.h"

#include <algorithm>

Parser::Parser() {}

std::vector<std::string> Parser::trimAndSkipComments(std::ifstream& vmFile) {

    std::string line;

    while(std::getline(vmFile, line)) {

        line = checkForComments(line);

        if(!line.empty() && line[0] != '\r') {
            m_vmBuff.push_back(line);
        }
    }

    vmFile.close();
    return m_vmBuff;
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

Parser::command_t Parser::commandType(const std::string& codeLine) const {

    try {
        auto pos = codeLine.find(' ');
        return m_commandStamps.at(codeLine.substr(0, pos));
    }
    catch (...) {
        return C_ARITHMETIC;
    }
}

void Parser::trimReturn(std::string& argRaw) const {

    argRaw.erase(std::remove(argRaw.begin(), argRaw.end(), '\r'), argRaw.end());
}

std::string Parser::arg1(const std::string& codeLine) const {

    auto firstSpace = codeLine.find(' ');
    auto secondSpace = codeLine.find(' ', firstSpace + 1);
    auto arg1 = codeLine.substr(firstSpace + 1, secondSpace - firstSpace - 1);
    trimReturn(arg1);

    return arg1;
}

std::string Parser::arg2(const std::string& codeLine) const {

    auto pos = codeLine.rfind(' ');
    auto lenght = codeLine.length() - (pos + 1);
    auto arg2 = codeLine.substr(pos + 1, lenght);
    trimReturn(arg2);

    return arg2;
}
