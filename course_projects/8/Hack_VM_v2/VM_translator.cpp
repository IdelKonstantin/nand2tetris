#include "VM_translator.h"
#include <experimental/filesystem>
#include <algorithm>

namespace fs = std::experimental::filesystem;

VMtranslator::VMtranslator(const std::string& asmFilename, std::vector<std::string> filesForParsing) :
    m_asmFilename(asmFilename), m_filesForParsing(filesForParsing) {}

VMtranslator::~VMtranslator() {

    if(m_asmFile.is_open()) {
        m_asmFile.close();
    }
}

bool VMtranslator::init() {

    m_asmFile.open(m_asmFilename);

    if(!m_asmFile.is_open()) {
        std::cerr << "Can't open asm file:" << m_asmFilename << " for writing" << std::endl;
        return false;
    }

    return true;
}

void VMtranslator::translateToAsmCommands(const std::vector<std::pair<std::string, std::string>>& vmBuff) {

    m_asmFile << m_code.writeBootStrap();

    for(auto& pair : vmBuff) {

        m_currentWorkingModule = pair.first;
        auto vmLine = pair.second;

        m_asmFile << "// " << vmLine << '\n';
        auto commandType = m_parser.commandType(vmLine);

        std::string arg1{};
        std::string arg2{};

        if(commandType != Parser::C_RETURN) {

            arg1 = m_parser.arg1(vmLine);

            if(commandType == Parser::C_PUSH || commandType == Parser::C_POP || commandType == Parser::C_FUNCTION || commandType == Parser::C_CALL) {
                arg2 = m_parser.arg2(vmLine);
            }
        }

        if(commandType == Parser::C_PUSH || commandType == Parser::C_POP) {
            m_asmFile << m_code.writePushPop(commandType, arg1, arg2, m_currentWorkingModule) << '\n';
        }
        else if(commandType == Parser::C_ARITHMETIC) {
            m_asmFile << m_code.writeArithmetic(arg1, m_currentWorkingModule) << '\n';
        }
        else if(commandType == Parser::C_LABEL) {
            m_asmFile << m_code.writeLable(arg1, m_currentWorkingModule) << '\n';
        }
        else if(commandType == Parser::C_GOTO) {
            m_asmFile << m_code.writeGoto(arg1, m_currentWorkingModule) << '\n';
        }
        else if(commandType == Parser::C_IF) {
            m_asmFile << m_code.writeIf(arg1, m_currentWorkingModule) << '\n';
        }
        else if(commandType == Parser::C_FUNCTION) {
            m_asmFile << m_code.writeFunction(arg1, arg2, m_currentWorkingModule) << '\n';
        }
        else if(commandType == Parser::C_RETURN) {
            m_asmFile << m_code.writeReturn() << '\n';
        }
        else if(commandType == Parser::C_CALL) {
            m_asmFile << m_code.writeCall(arg1, arg2) << '\n';
        }
    }
}

void VMtranslator::translateAndSaveAll() {

    std::vector<std::pair<std::string, std::string>> vmBuff;

    for(const auto& filePath : m_filesForParsing) {

        m_VMFile.open(filePath);
        auto fileContent = m_parser.trimAndSkipComments(m_VMFile);
        m_VMFile.close();

        auto fileName = fs::path{filePath}.stem().string();

        for(const auto& line : fileContent) {
            vmBuff.push_back({fileName, line});
        }
    }

    translateToAsmCommands(vmBuff);
}
