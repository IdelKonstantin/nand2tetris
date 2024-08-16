#include "assembler.h"

Assembler::Assembler(const char* asmPath) : m_asmFilePath(asmPath) {
    m_hackFilename = m_asmFilePath.stem().string() + std::string(".hack");
}

Assembler::~Assembler() {

    if(m_hackFile.is_open()) {
        m_hackFile.close();
    }
}

void Assembler::saveLablesToSymbolTable(const std::vector<std::string>& asmBuff) {

    auto i = 0;

    for(auto& asmLine : asmBuff) {

        auto instructionType = m_parser.instructionType(asmLine);

        if(instructionType == Parser::L_INSTRUCTION) {
            m_sTable.addLableIfNotExists({m_parser.symbol(asmLine), i});
        }
        i++;
    }
}

void Assembler::saveVariablesToSymbolTable(const std::vector<std::string>& asmBuff) {

    auto i = 0;

    for(auto& asmLine : asmBuff) {

        auto instructionType = m_parser.instructionType(asmLine);

        if(instructionType == Parser::A_INSTRUCTION) {
            m_sTable.addVariabeIfNotExists({m_parser.symbol(asmLine)});
        }
        i++;
    }
}

void Assembler::translateToMachineCode(const std::vector<std::string>& asmBuff) {

    for(auto& asmLine : asmBuff) {

        std::string outputLine;

        auto instructionType = m_parser.instructionType(asmLine);

        if(instructionType == Parser::A_INSTRUCTION) {

            outputLine = "0";

            try {

                std::bitset<15> binaryAvalue(std::stoi(m_parser.symbol(asmLine)));
                outputLine += binaryAvalue.to_string();
            }
            catch (...) {
                auto addr = m_sTable.getData().at(m_parser.symbol(asmLine));

                std::bitset<15> binaryAvalue(addr);
                outputLine += binaryAvalue.to_string();
            }
        }
        else if(instructionType == Parser::C_INSTRUCTION) {

            outputLine = "111";
            outputLine += m_code.comp(m_parser.comp(asmLine));
            outputLine += m_code.dest(m_parser.dest(asmLine));
            outputLine += m_code.jump(m_parser.jump(asmLine));
        }

        if(!outputLine.empty()) {
            m_hackFile << outputLine << '\n';
        }
    }
}

bool Assembler::init() {
    m_asmFile.open(m_asmFilePath.string());

    if(!m_asmFile.is_open()) {
        std::cerr << "Can't open asm file:" << m_asmFilePath.string() << std::endl;
        return false;
    }

    m_hackFile.open(m_hackFilename);

    if(!m_hackFile.is_open()) {
        std::cerr << "Can't open hack file:" << m_hackFilename << " for writing" << std::endl;
        return false;
    }

    return true;
}

void Assembler::translateAndSave() {

    auto asmBuff = m_parser.trimAndSkipComments(m_asmFile);

    //1) Cоздание таблицы символов (первый проход транслятора) - поиск меток (LABLE)
    saveLablesToSymbolTable(asmBuff);

    //2) Cоздание таблицы символов (второй проход транслятора) - поиск переменных @xxx
    saveVariablesToSymbolTable(asmBuff);

    //3) Преобразование символов в машинный код
    translateToMachineCode(asmBuff);
}

void Assembler::showSymbolTable() const {

    for(const auto& symbolPair : m_sTable.getData()) {
        std::cout << symbolPair.first << " : " << symbolPair.second << std::endl;
    }
}
