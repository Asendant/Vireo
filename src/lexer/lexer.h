#pragma once
#ifndef LEXER_H
#define LEXER_H
#include <string>
#include <vector>
#include "tokentype.h"

namespace Lexer {
    class Lexer {
    private:
        std::string sourceCode;

        int start = 0;
        int current = 0;

        int currentLine = 1;
        int currentChar = 0;
        std::vector<TokenType> tokens;
    public:
        Lexer(const std::string& code);
        std::vector<TokenType> Tokenize();
    };
}

#endif //LEXER_H
