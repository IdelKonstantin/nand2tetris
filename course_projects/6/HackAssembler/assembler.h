#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <fstream>
#include <vector>
#include <algorithm>
#include <bitset>
#include <iostream>

#include <experimental/filesystem>

#include "parser.h"
#include "code.h"
#include "symboltable.h"

using namespace std::experimental::filesystem;

class Assembler {

private:
    path m_asmFilePath;
    std::string m_hackFilename;

    std::ifstream m_asmFile;
    std::ofstream m_hackFile;

private:

    Parser m_parser;
    Code m_code;
    SymbolTable m_sTable;

private:

    void saveLablesToSymbolTable(const std::vector<std::string>& asmBuff);
    void saveVariablesToSymbolTable(const std::vector<std::string>& asmBuff);
    void translateToMachineCode(const std::vector<std::string>& asmBuff);

public:
    Assembler(const char* asmPath);
    ~Assembler();

    bool init();

    void translateAndSave();
    void showSymbolTable() const;
};

#endif // ASSEMBLER_H
