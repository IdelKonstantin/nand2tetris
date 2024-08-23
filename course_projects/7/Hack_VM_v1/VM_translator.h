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
    path m_VMFilePath;
    std::string m_asmFilename;
    std::string m_currentWoringModule;

    std::ifstream m_VMFile;
    std::ofstream m_asmFile;

private:

    Parser m_parser;
    CodeWriter m_code;

private:

    void translateToAsmCommands(const std::vector<std::string>& vmBuff);

public:
    VMtranslator(const char* VMFilePath);
    ~VMtranslator();

    bool init();

    void translateAndSave();
};

#endif // ASSEMBLER_H
