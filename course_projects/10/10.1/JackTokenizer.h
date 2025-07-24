#ifndef JACKTOKENIZER_H
#define JACKTOKENIZER_H

#include <string>
#include <vector>
#include <fstream>
#include <unordered_set>

enum class TokenType {
    KEYWORD,
    SYMBOL,
    IDENTIFIER,
    INT_CONST,
    STRING_CONST
};

enum class Keyword {
    CLASS, METHOD, FUNCTION, CONSTRUCTOR,
    INT, BOOLEAN, CHAR, VOID,
    VAR, STATIC, FIELD,
    LET, DO, IF, ELSE, WHILE, RETURN,
    TRUE, FALSE, NULL_, THIS
};

class JackTokenizer {
public:
    JackTokenizer(const std::string& inputFile);
    bool hasMoreTokens();
    void advance();

    TokenType tokenType();
    Keyword keyword();
    char symbol();
    std::string identifier();
    int intVal();
    std::string stringVal();
    void writeTokensToXML();

private:
    void removeComments();
    void tokenize();
    bool isSymbol(char c);
    Keyword stringToKeyword(const std::string& str);

    std::ifstream input;
    std::string inputFilePath;
    std::string currentToken;
    TokenType currentTokenType;
    std::vector<std::string> tokens;
    size_t currentIndex;
};

#endif
