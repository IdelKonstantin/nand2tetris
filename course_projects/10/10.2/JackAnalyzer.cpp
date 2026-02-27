#include "JackAnalyzer.h"
#include <stdexcept>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

JackAnalyzer::JackAnalyzer(const std::string& inputFile) : indentLevel(0) {
    tokenizer = std::make_unique<JackTokenizer>(inputFile);
    
    fs::path inputPath(inputFile);
    std::string outputFile = inputPath.replace_extension("my.xml").string();
    
    this->outputFile.open(outputFile);
    if (!this->outputFile.is_open())
        throw std::runtime_error("Cannot create output file: " + outputFile);
}

void JackAnalyzer::openTag(const std::string& tag) {
    outputFile << std::string(indentLevel * 2, ' ') << "<" << tag << ">\n";
    indentLevel++;
}

void JackAnalyzer::closeTag(const std::string& tag) {
    indentLevel--;
    outputFile << std::string(indentLevel * 2, ' ') << "</" << tag << ">\n";
}

void JackAnalyzer::writeCurrentToken() {
    std::string indent = std::string(indentLevel * 2, ' ');
    
    switch (tokenizer->tokenType()) {
        case TokenType::KEYWORD:
            outputFile << indent << "<keyword> " << tokenizer->identifier() << " </keyword>\n";
            break;
        case TokenType::SYMBOL: {
            char sym = tokenizer->symbol();
            std::string escaped;
            switch (sym) {
                case '<': escaped = "&lt;"; break;
                case '>': escaped = "&gt;"; break;
                case '&': escaped = "&amp;"; break;
                case '"': escaped = "&quot;"; break;
                default: escaped = std::string(1, sym);
            }
            outputFile << indent << "<symbol> " << escaped << " </symbol>\n";
            break;
        }
        case TokenType::IDENTIFIER:
            outputFile << indent << "<identifier> " << tokenizer->identifier() << " </identifier>\n";
            break;
        case TokenType::INT_CONST:
            outputFile << indent << "<integerConstant> " << tokenizer->intVal() << " </integerConstant>\n";
            break;
        case TokenType::STRING_CONST:
            outputFile << indent << "<stringConstant> " << tokenizer->stringVal() << " </stringConstant>\n";
            break;
    }
    tokenizer->advance();
}

void JackAnalyzer::eat(TokenType expectedType) {
    if (tokenizer->tokenType() != expectedType)
        throw std::runtime_error("Expected token type");
    writeCurrentToken();
}

void JackAnalyzer::eatKeyword(const std::string& keyword) {
    if (tokenizer->tokenType() != TokenType::KEYWORD || tokenizer->identifier() != keyword)
        throw std::runtime_error("Expected keyword: " + keyword);
    writeCurrentToken();
}

void JackAnalyzer::eatSymbol(char symbol) {
    if (tokenizer->tokenType() != TokenType::SYMBOL || tokenizer->symbol() != symbol)
        throw std::runtime_error("Expected symbol: " + std::string(1, symbol));
    writeCurrentToken();
}

void JackAnalyzer::eatIdentifier() {
    if (tokenizer->tokenType() != TokenType::IDENTIFIER)
        throw std::runtime_error("Expected identifier");
    writeCurrentToken();
}

bool JackAnalyzer::peekKeyword(const std::string& keyword) {
    return tokenizer->tokenType() == TokenType::KEYWORD && tokenizer->identifier() == keyword;
}

bool JackAnalyzer::peekSymbol(char symbol) {
    return tokenizer->tokenType() == TokenType::SYMBOL && tokenizer->symbol() == symbol;
}

bool JackAnalyzer::peekType(TokenType type) {
    return tokenizer->tokenType() == type;
}

void JackAnalyzer::compileClass() {
    openTag("class");
    
    eatKeyword("class");
    eatIdentifier();
    eatSymbol('{');
    
    while (tokenizer->hasMoreTokens() && !peekSymbol('}')) {
        if (peekKeyword("static") || peekKeyword("field"))
            compileClassVarDec();
        else if (peekKeyword("constructor") || peekKeyword("function") || peekKeyword("method"))
            compileSubroutine();
        else
            throw std::runtime_error("Unexpected token in class");
    }
    
    eatSymbol('}');
    closeTag("class");
}

void JackAnalyzer::compileClassVarDec() {
    openTag("classVarDec");
    
    writeCurrentToken(); // static|field
    writeCurrentToken(); // type
    eatIdentifier(); // varName
    
    while (peekSymbol(',')) {
        eatSymbol(',');
        eatIdentifier();
    }
    
    eatSymbol(';');
    closeTag("classVarDec");
}

void JackAnalyzer::compileSubroutine() {
    openTag("subroutineDec");
    
    writeCurrentToken(); // constructor|function|method
    writeCurrentToken(); // void|type
    eatIdentifier(); // subroutineName
    
    eatSymbol('(');
    compileParameterList();
    eatSymbol(')');
    
    openTag("subroutineBody");
    eatSymbol('{');
    
    while (peekKeyword("var"))
        compileVarDec();
    
    compileStatements();
    
    eatSymbol('}');
    closeTag("subroutineBody");
    closeTag("subroutineDec");
}

void JackAnalyzer::compileParameterList() {
    openTag("parameterList");
    
    if (!peekSymbol(')')) {
        writeCurrentToken(); // type
        eatIdentifier(); // varName
        
        while (peekSymbol(',')) {
            eatSymbol(',');
            writeCurrentToken(); // type
            eatIdentifier(); // varName
        }
    }
    
    closeTag("parameterList");
}

void JackAnalyzer::compileVarDec() {
    openTag("varDec");
    
    eatKeyword("var");
    writeCurrentToken(); // type
    eatIdentifier(); // varName
    
    while (peekSymbol(',')) {
        eatSymbol(',');
        eatIdentifier();
    }
    
    eatSymbol(';');
    closeTag("varDec");
}

void JackAnalyzer::compileStatements() {
    openTag("statements");
    
    while (tokenizer->hasMoreTokens() && !peekSymbol('}')) {
        if (peekKeyword("let"))
            compileLet();
        else if (peekKeyword("if"))
            compileIf();
        else if (peekKeyword("while"))
            compileWhile();
        else if (peekKeyword("do"))
            compileDo();
        else if (peekKeyword("return"))
            compileReturn();
        else
            break;
    }
    
    closeTag("statements");
}

void JackAnalyzer::compileLet() {
    openTag("letStatement");
    
    eatKeyword("let");
    eatIdentifier(); // varName
    
    if (peekSymbol('[')) {
        eatSymbol('[');
        compileExpression();
        eatSymbol(']');
    }
    
    eatSymbol('=');
    compileExpression();
    eatSymbol(';');
    
    closeTag("letStatement");
}

void JackAnalyzer::compileIf() {
    openTag("ifStatement");
    
    eatKeyword("if");
    eatSymbol('(');
    compileExpression();
    eatSymbol(')');
    eatSymbol('{');
    compileStatements();
    eatSymbol('}');
    
    if (peekKeyword("else")) {
        eatKeyword("else");
        eatSymbol('{');
        compileStatements();
        eatSymbol('}');
    }
    
    closeTag("ifStatement");
}

void JackAnalyzer::compileWhile() {
    openTag("whileStatement");
    
    eatKeyword("while");
    eatSymbol('(');
    compileExpression();
    eatSymbol(')');
    eatSymbol('{');
    compileStatements();
    eatSymbol('}');
    
    closeTag("whileStatement");
}

void JackAnalyzer::compileDo() {
    openTag("doStatement");
    
    eatKeyword("do");
    eatIdentifier(); // name
    
    if (peekSymbol('.')) {
        eatSymbol('.');
        eatIdentifier(); // subroutine name
    }
    
    eatSymbol('(');
    compileExpressionList();
    eatSymbol(')');
    eatSymbol(';');
    
    closeTag("doStatement");
}

void JackAnalyzer::compileReturn() {
    openTag("returnStatement");
    
    eatKeyword("return");
    
    if (!peekSymbol(';'))
        compileExpression();
    
    eatSymbol(';');
    
    closeTag("returnStatement");
}

void JackAnalyzer::compileExpression() {
    openTag("expression");
    
    compileTerm();
    
    while (peekSymbol('+') || peekSymbol('-') || peekSymbol('*') || 
           peekSymbol('/') || peekSymbol('&') || peekSymbol('|') || 
           peekSymbol('<') || peekSymbol('>') || peekSymbol('=')) {
        writeCurrentToken(); // op
        compileTerm();
    }
    
    closeTag("expression");
}

void JackAnalyzer::compileTerm() {
    openTag("term");
    
    if (peekType(TokenType::INT_CONST)) {
        writeCurrentToken();
    }
    else if (peekType(TokenType::STRING_CONST)) {
        writeCurrentToken();
    }
    else if (peekKeyword("true") || peekKeyword("false") || 
             peekKeyword("null") || peekKeyword("this")) {
        writeCurrentToken();
    }
    else if (peekType(TokenType::IDENTIFIER)) {
        eatIdentifier(); // varName or subroutineName
        
        if (peekSymbol('[')) { // array[index]
            eatSymbol('[');
            compileExpression();
            eatSymbol(']');
        }
        else if (peekSymbol('(')) { // subroutineCall
            eatSymbol('(');
            compileExpressionList();
            eatSymbol(')');
        }
        else if (peekSymbol('.')) { // className.subroutineName
            eatSymbol('.');
            eatIdentifier(); // subroutineName
            eatSymbol('(');
            compileExpressionList();
            eatSymbol(')');
        }
        // else: просто переменная
    }
    else if (peekSymbol('(')) {
        eatSymbol('(');
        compileExpression();
        eatSymbol(')');
    }
    else if (peekSymbol('-') || peekSymbol('~')) { // unary op
        writeCurrentToken();
        compileTerm();
    }
    else {
        throw std::runtime_error("Expected term");
    }
    
    closeTag("term");
}

void JackAnalyzer::compileExpressionList() {
    openTag("expressionList");
    
    if (!peekSymbol(')')) {
        compileExpression();
        
        while (peekSymbol(',')) {
            eatSymbol(',');
            compileExpression();
        }
    }
    
    closeTag("expressionList");
}

void JackAnalyzer::writeXML() {
    if (tokenizer->hasMoreTokens()) {
        tokenizer->advance();
        compileClass();
    }
    outputFile.close();
}