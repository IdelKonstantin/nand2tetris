#ifndef VM_TRANS_H
#define VM_TRANS_H

#include <fstream>
#include <vector>
#include <algorithm>
#include <bitset>
#include <iostream>

#include <experimental/filesystem>

#include "parser.h"
#include "code_writer.h"

using namespace std::experimental::filesystem;

class VMtranslator {

private:
    std::string m_asmFilename;
    std::vector<std::string> m_filesForParsing;

    std::string m_currentWorkingModule;
    std::ifstream m_VMFile;
    std::ofstream m_asmFile;

private:

    Parser m_parser;
    CodeWriter m_code;

private:

    void translateToAsmCommands(const std::vector<std::pair<std::string, std::string>>& vmBuff);

public:
    VMtranslator(const std::string& asmFilename, std::vector<std::string> filesForParsing);
    ~VMtranslator();

    bool init();

    void translateAndSaveAll();
};

#endif // ASSEMBLER_H
