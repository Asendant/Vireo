#pragma once
#ifndef LEXER_H
#define LEXER_H
#include <string>
#include <vector>
#include "token.h"

namespace Lexer {
    class Lexer {
    private:
        std::string sourceCode;

        int start = 0;
        int current = 0;

        int currentLine = 1;
        int currentChar = 0; // Used for error logging if the programmer makes an error. Shows the exact character.
        std::vector<Token> tokens;
    public:
        // Main Functions
        explicit Lexer(const std::string& filePath);
        std::vector<Token> Tokenize();

        // Helper Functions
        static std::string ConvertSourceToString(const std::string& filePath);
        void ScanToken();
        char Advance();
        void Number();
        void Identifier();
        void AddToken(TokenType type);
    };
}

#endif //LEXER_H
