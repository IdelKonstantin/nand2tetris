#include "VM_translator.h"

VMtranslator::VMtranslator(const char* VMFilePath) : m_VMFilePath(VMFilePath) {

    m_currentWoringModule = m_VMFilePath.stem().string();
    m_asmFilename = m_currentWoringModule + std::string(".asm");
}

VMtranslator::~VMtranslator() {

    if(m_asmFile.is_open()) {
        m_asmFile.close();
    }
}

bool VMtranslator::init() {
    m_VMFile.open(m_VMFilePath.string());

    if(!m_VMFile.is_open()) {
        std::cerr << "Can't open vm file:" << m_VMFilePath.string() << std::endl;
        return false;
    }

    m_asmFile.open(m_asmFilename);

    if(!m_asmFile.is_open()) {
        std::cerr << "Can't open asm file:" << m_asmFilename << " for writing" << std::endl;
        return false;
    }

    return true;
}

void VMtranslator::translateToAsmCommands(const std::vector<std::string>& vmBuff) {

    for(auto& vmLine : vmBuff) {

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
            m_asmFile << m_code.writePushPop(commandType, arg1, arg2, m_currentWoringModule) << '\n';
        }
        else if(commandType == Parser::C_ARITHMETIC) {
            m_asmFile << m_code.writeArithmetic(arg1, m_currentWoringModule) << '\n';
        }
        //TODO: else if() //call, return, function etc...
    }
}

void VMtranslator::translateAndSave() {

    auto vmBuff = m_parser.trimAndSkipComments(m_VMFile);
    translateToAsmCommands(vmBuff);
}
