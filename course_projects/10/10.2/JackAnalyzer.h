#ifndef JACKANALYZER_H
#define JACKANALYZER_H

#include "JackTokenizer.h"
#include <string>
#include <memory>
#include <fstream>

class JackAnalyzer {
public:
    JackAnalyzer(const std::string& inputFile);
    void compileClass();
    void writeXML();

private:
    void eat(TokenType expectedType);
    void eatKeyword(const std::string& keyword);
    void eatSymbol(char symbol);
    void eatIdentifier();
    bool peekKeyword(const std::string& keyword);
    bool peekSymbol(char symbol);
    bool peekType(TokenType type);
    
    void openTag(const std::string& tag);
    void closeTag(const std::string& tag);
    void writeCurrentToken();
    
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
    void compileExpressionList();

    std::unique_ptr<JackTokenizer> tokenizer;
    std::ofstream outputFile;
    int indentLevel;
};

#endif