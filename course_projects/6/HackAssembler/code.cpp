#include "code.h"

Code::Code() {}

std::string Code::dest(const std::string& destString) const {
    return m_destCodes.at(destString);
}

std::string Code::comp(const std::string& compString) const {
    return m_compCodes.at(compString);
}

std::string Code::jump(const std::string& jumpString) const {
    return m_jumpCodes.at(jumpString);
}
