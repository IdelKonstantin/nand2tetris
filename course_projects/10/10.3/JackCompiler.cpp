#include "JackCompiler.h"
#include <stdexcept>
#include <experimental/filesystem>
#include <iostream>

namespace fs = std::experimental::filesystem;

JackCompiler::JackCompiler(const std::string& inputFile) 
    : staticIndex(0), fieldIndex(0), argIndex(0), varIndex(0), labelCounter(0) {
    tokenizer = std::make_unique<JackTokenizer>(inputFile);
    
    fs::path inputPath(inputFile);
    std::string outputFile = inputPath.replace_extension("vm").string();
    
    this->outputFile.open(outputFile);
    if (!this->outputFile.is_open())
        throw std::runtime_error("Cannot create output file: " + outputFile);
}

std::string JackCompiler::newLabel() {
    return "L" + std::to_string(labelCounter++);
}

void JackCompiler::writeVM(const std::string& command) {
    outputFile << command << "\n";
}

void JackCompiler::writeVM(const std::string& command, const std::string& segment, int index) {
    outputFile << command << " " << segment << " " << index << "\n";
}

void JackCompiler::writeVM(const std::string& command, const std::string& label) {
    outputFile << command << " " << label << "\n";
}

void JackCompiler::defineSymbol(const std::string& name, const std::string& type, const std::string& kind) {
    Symbol sym;
    sym.name = name;
    sym.type = type;
    sym.kind = kind;
    
    if (kind == "static") {
        sym.index = staticIndex++;
        classTable[name] = sym;
    } else if (kind == "field") {
        sym.index = fieldIndex++;
        classTable[name] = sym;
    } else if (kind == "arg") {
        sym.index = argIndex++;
        subroutineTable[name] = sym;
    } else if (kind == "var") {
        sym.index = varIndex++;
        subroutineTable[name] = sym;
    }
}

void JackCompiler::defineSubroutine(const std::string& name, const std::string& returnType, const std::string& kind) {
    subroutineName = name;
    subroutineType = kind;
    subroutineTable.clear();
    argIndex = 0;
    varIndex = 0;
    
    // Для метода первый аргумент - this
    if (kind == "method") {
        Symbol thisSym;
        thisSym.name = "this";
        thisSym.type = className;
        thisSym.kind = "arg";
        thisSym.index = argIndex++;
        subroutineTable["this"] = thisSym;
    }
}

int JackCompiler::varCount(const std::string& kind) {
    if (kind == "static") return staticIndex;
    if (kind == "field") return fieldIndex;
    if (kind == "arg") return argIndex;
    if (kind == "var") return varIndex;
    return 0;
}

std::pair<std::string, int> JackCompiler::getSymbol(const std::string& name) {
    // Сначала ищем в subroutineTable, потом в classTable
    if (subroutineTable.find(name) != subroutineTable.end()) {
        Symbol sym = subroutineTable[name];
        return {sym.kind, sym.index};
    }
    if (classTable.find(name) != classTable.end()) {
        Symbol sym = classTable[name];
        std::string segment = (sym.kind == "field") ? "this" : "static";
        return {segment, sym.index};
    }
    throw std::runtime_error("Undefined symbol: " + name);
}

void JackCompiler::eat(TokenType expectedType) {
    if (tokenizer->tokenType() != expectedType)
        throw std::runtime_error("Expected token type");
    tokenizer->advance();
}

void JackCompiler::eatKeyword(const std::string& keyword) {
    if (tokenizer->tokenType() != TokenType::KEYWORD || tokenizer->identifier() != keyword)
        throw std::runtime_error("Expected keyword: " + keyword);
    tokenizer->advance();
}

void JackCompiler::eatSymbol(char symbol) {
    if (tokenizer->tokenType() != TokenType::SYMBOL || tokenizer->symbol() != symbol)
        throw std::runtime_error("Expected symbol: " + std::string(1, symbol));
    tokenizer->advance();
}

void JackCompiler::eatIdentifier() {
    if (tokenizer->tokenType() != TokenType::IDENTIFIER)
        throw std::runtime_error("Expected identifier");
    tokenizer->advance();
}

bool JackCompiler::peekKeyword(const std::string& keyword) {
    return tokenizer->tokenType() == TokenType::KEYWORD && tokenizer->identifier() == keyword;
}

bool JackCompiler::peekSymbol(char symbol) {
    return tokenizer->tokenType() == TokenType::SYMBOL && tokenizer->symbol() == symbol;
}

bool JackCompiler::peekType(TokenType type) {
    return tokenizer->tokenType() == type;
}

void JackCompiler::compileClass() {
    eatKeyword("class");
    className = tokenizer->identifier();
    eatIdentifier();
    eatSymbol('{');
    
    staticIndex = 0;
    fieldIndex = 0;
    classTable.clear();
    
    while (tokenizer->hasMoreTokens() && !peekSymbol('}')) {
        if (peekKeyword("static") || peekKeyword("field"))
            compileClassVarDec();
        else if (peekKeyword("constructor") || peekKeyword("function") || peekKeyword("method"))
            compileSubroutine();
        else
            throw std::runtime_error("Unexpected token in class");
    }
    
    eatSymbol('}');
}

void JackCompiler::compileClassVarDec() {
    std::string kind = tokenizer->identifier(); // static или field
    tokenizer->advance();
    
    std::string type = tokenizer->identifier();
    tokenizer->advance();
    
    std::string name = tokenizer->identifier();
    tokenizer->advance();
    defineSymbol(name, type, kind);
    
    while (peekSymbol(',')) {
        eatSymbol(',');
        name = tokenizer->identifier();
        tokenizer->advance();
        defineSymbol(name, type, kind);
    }
    
    eatSymbol(';');
}

void JackCompiler::compileSubroutine() {
    std::string kind = tokenizer->identifier(); // constructor, function, method
    tokenizer->advance();
    
    std::string returnType = tokenizer->identifier(); // void или тип
    tokenizer->advance();
    
    std::string name = tokenizer->identifier();
    tokenizer->advance();
    defineSubroutine(name, returnType, kind);
    
    eatSymbol('(');
    compileParameterList();
    eatSymbol(')');
    
    eatSymbol('{');
    
    while (peekKeyword("var"))
        compileVarDec();
    
    // VM: function className.subroutineName nVars
    writeVM("function", className + "." + subroutineName, varCount("var"));
    
    if (kind == "constructor") {
        // Выделяем память для объекта
        writeVM("push", "constant", fieldIndex);
        writeVM("call", "Memory.alloc", 1);
        writeVM("pop", "pointer", 0); // this = new object
    } else if (kind == "method") {
        // Аргумент 0 - это this
        writeVM("push", "argument", 0);
        writeVM("pop", "pointer", 0); // this = argument 0
    }
    
    compileStatements();
    
    eatSymbol('}');
}

void JackCompiler::compileParameterList() {
    if (!peekSymbol(')')) {
        std::string type = tokenizer->identifier();
        tokenizer->advance();
        
        std::string name = tokenizer->identifier();
        tokenizer->advance();
        defineSymbol(name, type, "arg");
        
        while (peekSymbol(',')) {
            eatSymbol(',');
            type = tokenizer->identifier();
            tokenizer->advance();
            
            name = tokenizer->identifier();
            tokenizer->advance();
            defineSymbol(name, type, "arg");
        }
    }
}

void JackCompiler::compileVarDec() {
    eatKeyword("var");
    
    std::string type = tokenizer->identifier();
    tokenizer->advance();
    
    std::string name = tokenizer->identifier();
    tokenizer->advance();
    defineSymbol(name, type, "var");
    
    while (peekSymbol(',')) {
        eatSymbol(',');
        name = tokenizer->identifier();
        tokenizer->advance();
        defineSymbol(name, type, "var");
    }
    
    eatSymbol(';');
}

void JackCompiler::compileStatements() {
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
}

void JackCompiler::compileLet() {
    eatKeyword("let");
    
    std::string varName = tokenizer->identifier();
    tokenizer->advance();
    
    bool isArray = false;
    if (peekSymbol('[')) {
        isArray = true;
        eatSymbol('[');
        compileExpression(); // вычисляем индекс
        eatSymbol(']');
        
        // Адрес: base + index
        auto sym = getSymbol(varName);
        writeVM("push", sym.first, sym.second);
        writeVM("add"); // this + index или static + index
    }
    
    eatSymbol('=');
    compileExpression();
    
    if (isArray) {
        // Сохраняем значение по адресу
        writeVM("pop", "temp", 0); // значение в temp 0
        writeVM("pop", "pointer", 1); // адрес в THAT
        writeVM("push", "temp", 0);
        writeVM("pop", "that", 0);
    } else {
        auto sym = getSymbol(varName);
        writeVM("pop", sym.first, sym.second);
    }
    
    eatSymbol(';');
}

void JackCompiler::compileIf() {
    eatKeyword("if");
    eatSymbol('(');
    compileExpression();
    eatSymbol(')');
    
    std::string label1 = newLabel();
    std::string label2 = newLabel();
    
    writeVM("not"); // инвертируем условие
    writeVM("if-goto", label1);
    
    eatSymbol('{');
    compileStatements();
    eatSymbol('}');
    
    writeVM("goto", label2);
    writeVM("label", label1);
    
    if (peekKeyword("else")) {
        eatKeyword("else");
        eatSymbol('{');
        compileStatements();
        eatSymbol('}');
    }
    
    writeVM("label", label2);
}

void JackCompiler::compileWhile() {
    eatKeyword("while");
    
    std::string label1 = newLabel();
    std::string label2 = newLabel();
    
    writeVM("label", label1);
    
    eatSymbol('(');
    compileExpression();
    eatSymbol(')');
    
    writeVM("not");
    writeVM("if-goto", label2);
    
    eatSymbol('{');
    compileStatements();
    eatSymbol('}');
    
    writeVM("goto", label1);
    writeVM("label", label2);
}

void JackCompiler::compileDo() {
    eatKeyword("do");
    
    std::string name = tokenizer->identifier();
    tokenizer->advance();
    
    int nArgs = 0;
    
    if (peekSymbol('.')) {
        // method или function вызов
        eatSymbol('.');
        std::string subroutine = tokenizer->identifier();
        tokenizer->advance();
        
        // Проверяем, является ли name классом или переменной
        bool isMethodCall = false;
        std::string type;
        
        try {
            auto sym = getSymbol(name);
            type = sym.first; // на самом деле тут нужен тип переменной
            isMethodCall = true;
        } catch (...) {
            // Это вызов функции класса
            type = name;
        }
        
        eatSymbol('(');
        if (isMethodCall) {
            // Для метода передаем this как первый аргумент
            auto sym = getSymbol(name);
            writeVM("push", sym.first, sym.second);
            nArgs = 1;
        }
        nArgs += compileExpressionList();
        eatSymbol(')');
        
        if (isMethodCall) {
            writeVM("call", type + "." + subroutine, nArgs);
        } else {
            writeVM("call", name + "." + subroutine, nArgs);
        }
    } else {
        // Вызов метода этого объекта
        eatSymbol('(');
        writeVM("push", "pointer", 0); // this
        nArgs = 1 + compileExpressionList();
        eatSymbol(')');
        
        writeVM("call", className + "." + name, nArgs);
    }
    
    writeVM("pop", "temp", 0); // выкидываем возвращаемое значение
    eatSymbol(';');
}

void JackCompiler::compileReturn() {
    eatKeyword("return");
    
    if (!peekSymbol(';')) {
        compileExpression();
    } else {
        // return void - возвращаем 0
        writeVM("push", "constant", 0);
    }
    
    writeVM("return");
    eatSymbol(';');
}

void JackCompiler::compileExpression() {
    compileTerm();
    
    while (peekSymbol('+') || peekSymbol('-') || peekSymbol('*') || 
           peekSymbol('/') || peekSymbol('&') || peekSymbol('|') || 
           peekSymbol('<') || peekSymbol('>') || peekSymbol('=')) {
        char op = tokenizer->symbol();
        tokenizer->advance();
        compileTerm();
        
        switch (op) {
            case '+': writeVM("add"); break;
            case '-': writeVM("sub"); break;
            case '=': writeVM("eq"); break;
            case '>': writeVM("gt"); break;
            case '<': writeVM("lt"); break;
            case '&': writeVM("and"); break;
            case '|': writeVM("or"); break;
            case '*': writeVM("call", "Math.multiply", 2); break;
            case '/': writeVM("call", "Math.divide", 2); break;
        }
    }
}

void JackCompiler::compileTerm() {
    if (peekType(TokenType::INT_CONST)) {
        int val = tokenizer->intVal();
        tokenizer->advance();
        writeVM("push", "constant", val);
    }
    else if (peekType(TokenType::STRING_CONST)) {
        std::string str = tokenizer->stringVal();
        tokenizer->advance();
        
        writeVM("push", "constant", str.length());
        writeVM("call", "String.new", 1);
        
        for (char c : str) {
            writeVM("push", "constant", (int)c);
            writeVM("call", "String.appendChar", 2);
        }
    }
    else if (peekKeyword("true") || peekKeyword("false") || 
             peekKeyword("null") || peekKeyword("this")) {
        
        if (peekKeyword("true")) {
            writeVM("push", "constant", 0);
            writeVM("not");
        } else if (peekKeyword("false") || peekKeyword("null")) {
            writeVM("push", "constant", 0);
        } else if (peekKeyword("this")) {
            writeVM("push", "pointer", 0);
        }
        tokenizer->advance();
    }
    else if (peekType(TokenType::IDENTIFIER)) {
        std::string name = tokenizer->identifier();
        tokenizer->advance();
        
        if (peekSymbol('[')) { // array[index]
            auto sym = getSymbol(name);
            writeVM("push", sym.first, sym.second); // base address
            
            eatSymbol('[');
            compileExpression();
            eatSymbol(']');
            
            writeVM("add"); // base + index
            writeVM("pop", "pointer", 1); // that = address
            writeVM("push", "that", 0);
        }
        else if (peekSymbol('(')) { // subroutineCall
            eatSymbol('(');
            writeVM("push", "pointer", 0); // this
            int nArgs = 1 + compileExpressionList();
            eatSymbol(')');
            writeVM("call", className + "." + name, nArgs);
        }
        else if (peekSymbol('.')) { // className.subroutineName
            eatSymbol('.');
            std::string subroutine = tokenizer->identifier();
            tokenizer->advance();
            
            eatSymbol('(');
            int nArgs = compileExpressionList();
            eatSymbol(')');
            
            writeVM("call", name + "." + subroutine, nArgs);
        }
        else { // переменная
            auto sym = getSymbol(name);
            writeVM("push", sym.first, sym.second);
        }
    }
    else if (peekSymbol('(')) {
        eatSymbol('(');
        compileExpression();
        eatSymbol(')');
    }
    else if (peekSymbol('-') || peekSymbol('~')) {
        char op = tokenizer->symbol();
        tokenizer->advance();
        compileTerm();
        
        if (op == '-') {
            writeVM("neg");
        } else { // ~
            writeVM("not");
        }
    }
    else {
        throw std::runtime_error("Expected term");
    }
}

int JackCompiler::compileExpressionList() {
    int count = 0;
    
    if (!peekSymbol(')')) {
        compileExpression();
        count++;
        
        while (peekSymbol(',')) {
            eatSymbol(',');
            compileExpression();
            count++;
        }
    }
    
    return count;
}

void JackCompiler::writeVMCode() {
    if (tokenizer->hasMoreTokens()) {
        tokenizer->advance();
        compileClass();
    }
    outputFile.close();
}