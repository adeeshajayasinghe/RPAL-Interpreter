//
// Created by adees on 15/07/2023.
//

#ifndef UNTITLED1_LEXER_H
#define UNTITLED1_LEXER_H

#include <string>
#include <vector>
using namespace std;

enum TokenType {
    NUMBER,
    IDENTIFIER,
    OPERATOR,
    KEYWORD,
    STRING,
    COMMENT
};

struct Token {
    TokenType type;
    basic_string<char> lexeme;
};

vector<Token> lexicalAnalyzer(char* str);

#endif //UNTITLED1_LEXER_H
