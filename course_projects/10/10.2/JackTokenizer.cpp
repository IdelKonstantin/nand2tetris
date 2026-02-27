#include "JackTokenizer.h"
#include <sstream>
#include <regex>
#include <stdexcept>
#include <experimental/filesystem>
#include <fstream>

namespace fs = std::experimental::filesystem;

const std::unordered_set<std::string> keywordSet = {
    "class", "constructor", "function", "method", "field", "static", "var",
    "int", "char", "boolean", "void", "true", "false", "null", "this",
    "let", "do", "if", "else", "while", "return"
};

const std::unordered_set<char> symbols = {
    '{', '}', '(', ')', '[', ']', '.', ',', ';',
    '+', '-', '*', '/', '&', '|', '<', '>', '=', '~'
};

JackTokenizer::JackTokenizer(const std::string& inputFile)
    : inputFilePath(inputFile), input(inputFile), currentIndex(0) {
    if (!input.is_open()) throw std::runtime_error("Cannot open file");
    removeComments();
    tokenize();
}

bool JackTokenizer::hasMoreTokens() {
    return currentIndex < tokens.size();
}

void JackTokenizer::advance() {
    currentToken = tokens[currentIndex++];
    if (keywordSet.count(currentToken)) {
        currentTokenType = TokenType::KEYWORD;
    } else if (currentToken.size() == 1 && isSymbol(currentToken[0])) {
        currentTokenType = TokenType::SYMBOL;
    } else if (std::regex_match(currentToken, std::regex("[0-9]+"))) {
        currentTokenType = TokenType::INT_CONST;
    } else if (currentToken[0] == '"' && currentToken.back() == '"') {
        currentTokenType = TokenType::STRING_CONST;
    } else {
        currentTokenType = TokenType::IDENTIFIER;
    }
}

TokenType JackTokenizer::tokenType() {
    return currentTokenType;
}

Keyword JackTokenizer::keyword() {
    return stringToKeyword(currentToken);
}

char JackTokenizer::symbol() {
    return currentToken[0];
}

std::string JackTokenizer::identifier() {
    return currentToken;
}

int JackTokenizer::intVal() {
    return std::stoi(currentToken);
}

std::string JackTokenizer::stringVal() {
    return currentToken.substr(1, currentToken.size() - 2);
}

bool JackTokenizer::isSymbol(char c) {
    return symbols.count(c) > 0;
}

Keyword JackTokenizer::stringToKeyword(const std::string& str) {
    if (str == "class") return Keyword::CLASS;
    if (str == "constructor") return Keyword::CONSTRUCTOR;
    if (str == "function") return Keyword::FUNCTION;
    if (str == "method") return Keyword::METHOD;
    if (str == "field") return Keyword::FIELD;
    if (str == "static") return Keyword::STATIC;
    if (str == "var") return Keyword::VAR;
    if (str == "int") return Keyword::INT;
    if (str == "char") return Keyword::CHAR;
    if (str == "boolean") return Keyword::BOOLEAN;
    if (str == "void") return Keyword::VOID;
    if (str == "true") return Keyword::TRUE;
    if (str == "false") return Keyword::FALSE;
    if (str == "null") return Keyword::NULL_;
    if (str == "this") return Keyword::THIS;
    if (str == "let") return Keyword::LET;
    if (str == "do") return Keyword::DO;
    if (str == "if") return Keyword::IF;
    if (str == "else") return Keyword::ELSE;
    if (str == "while") return Keyword::WHILE;
    if (str == "return") return Keyword::RETURN;
    return Keyword::CLASS;
}

void JackTokenizer::removeComments() {
    std::stringstream buffer;
    buffer << input.rdbuf();
    std::string code = buffer.str();
    input.close();

    size_t pos = 0;
    while ((pos = code.find("/*", pos)) != std::string::npos) {
        size_t end = code.find("*/", pos + 2);
        if (end == std::string::npos) break;
        code.erase(pos, end - pos + 2);
    }

    pos = 0;
    while ((pos = code.find("//", pos)) != std::string::npos) {
        size_t end = code.find('\n', pos + 2);
        if (end == std::string::npos) {
            code.erase(pos);
        } else {
            code.erase(pos, end - pos);
        }
    }

    content.str(code);
    content.clear();
}

void JackTokenizer::tokenize() {
    std::string code = content.str();
    size_t i = 0;
    
    while (i < code.size()) {
        if (std::isspace(code[i])) {
            ++i;
        } else if (isSymbol(code[i])) {
            tokens.emplace_back(1, code[i]);
            ++i;
        } else if (code[i] == '"') {
            size_t j = i + 1;
            while (j < code.size() && code[j] != '"') ++j;
            tokens.push_back(code.substr(i, j - i + 1));
            i = j + 1;
        } else {
            size_t j = i;
            while (j < code.size() && !std::isspace(code[j]) && !isSymbol(code[j])) ++j;
            tokens.push_back(code.substr(i, j - i));
            i = j;
        }
    }
}

static std::string escapeSymbol(char symbol) {
    switch (symbol) {
        case '<': return "&lt;";
        case '>': return "&gt;";
        case '&': return "&amp;";
        case '"': return "&quot;";
        default: return std::string(1, symbol);
    }
}

void JackTokenizer::writeTokensToXML() {
    fs::path inputPath(inputFilePath);
    std::string outputFile = inputPath.replace_extension("T.xml").string();

    std::ofstream out(outputFile);
    if (!out.is_open()) throw std::runtime_error("Cannot write to file: " + outputFile);

    out << "<tokens>\n";
    currentIndex = 0;
    while (hasMoreTokens()) {
        advance();
        switch (currentTokenType) {
            case TokenType::KEYWORD:
                out << "  <keyword> " << currentToken << " </keyword>\n";
                break;
            case TokenType::SYMBOL:
                out << "  <symbol> " << escapeSymbol(symbol()) << " </symbol>\n";
                break;
            case TokenType::IDENTIFIER:
                out << "  <identifier> " << identifier() << " </identifier>\n";
                break;
            case TokenType::INT_CONST:
                out << "  <integerConstant> " << intVal() << " </integerConstant>\n";
                break;
            case TokenType::STRING_CONST:
                out << "  <stringConstant> " << stringVal() << " </stringConstant>\n";
                break;
            default:
                break;
        }
    }
    out << "</tokens>\n";
    out.close();
}