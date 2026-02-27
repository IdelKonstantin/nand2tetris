#ifndef JACKCOMPILER_H
#define JACKCOMPILER_H

#include "JackTokenizer.h"
#include <string>
#include <memory>
#include <fstream>
#include <stack>
#include <map>

class JackCompiler {
public:
    JackCompiler(const std::string& inputFile);
    void compileClass();
    void writeVMCode();

private:
    // Утилиты
    void eat(TokenType expectedType);
    void eatKeyword(const std::string& keyword);
    void eatSymbol(char symbol);
    void eatIdentifier();
    bool peekKeyword(const std::string& keyword);
    bool peekSymbol(char symbol);
    bool peekType(TokenType type);
    
    // Генерация VM кода
    void writeVM(const std::string& command);
    void writeVM(const std::string& command, const std::string& segment, int index);
    void writeVM(const std::string& command, const std::string& label);
    
    // Управление символами
    void defineSymbol(const std::string& name, const std::string& type, const std::string& kind);
    void defineSubroutine(const std::string& name, const std::string& returnType, const std::string& kind);
    int varCount(const std::string& kind);
    std::pair<std::string, int> getSymbol(const std::string& name);
    
    // Методы компиляции
    void compileClassVarDec();
    void compileSubroutine();
    void compileParameterList();
    void compileVarDec();
    void compileStatements();
    void compileLet();
    void compileIf();
    void compileWhile();
    void compileDo();
    void compileReturn();
    void compileExpression();
    void compileTerm();
    int compileExpressionList();

    std::unique_ptr<JackTokenizer> tokenizer;
    std::ofstream outputFile;
    std::string className;
    std::string subroutineName;
    std::string subroutineType; // constructor, function, method
    
    // Символьные таблицы
    struct Symbol {
        std::string name;
        std::string type;
        std::string kind; // static, field, arg, var
        int index;
    };
    
    std::map<std::string, Symbol> classTable;
    std::map<std::string, Symbol> subroutineTable;
    int staticIndex;
    int fieldIndex;
    int argIndex;
    int varIndex;
    
    // Для меток
    int labelCounter;
    std::string newLabel();
};

#endif