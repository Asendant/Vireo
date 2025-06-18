#pragma once
#ifndef LEXER_H
#define LEXER_H
#include <string>
#include <vector>
#include <unordered_map>
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

        // Operators
        // PLUS, MINUS, MUL, DIV,
        // EQUAL_TO, LESS, GREATER,
        // AND, OR, ASSIGN,
        // // Punctuation
        // LEFT_PAREN, RIGHT_PAREN,
        // LEFT_BRACE, RIGHT_BRACE,
        // COMMA, COLON, SEMICOLON, ARROW,

        // Maps keyword strings to their corresponding TokenType values
        const std::unordered_map<std::string, TokenType> keywordMap = {
            // Keywords
            {"function", TokenType::FUNCTION},
            {"if",       TokenType::IF},
            {"else",     TokenType::ELSE},
            {"else_if",  TokenType::ELSE_IF},
            {"while",    TokenType::WHILE},
            {"for",      TokenType::FOR},
            {"return",   TokenType::RETURN},
            {"var",      TokenType::VAR},

            // Types
            {"int",    TokenType::TYPE_INT},
            {"float",  TokenType::TYPE_FLOAT},
            {"string", TokenType::TYPE_STRING},
            {"bool",   TokenType::TYPE_BOOL},
            {"true", TokenType::BOOL_LITERAL},
            {"false", TokenType::BOOL_LITERAL},
        };
    public:
        // Main Functions
        explicit Lexer(const std::string& source, bool fromFile);
        std::vector<Token> Tokenize();

        // Helper Functions
        static std::string ConvertSourceToString(const std::string& filePath);
        void ScanToken();
        char Advance();
        void Number();
        void Identifier();
        void String();
        void AddToken(TokenType type, const std::string& lexeme = "");
        char Peek() const;
        char PeekNext() const;
        bool IsAtEnd() const;
    };
}

#endif //LEXER_H
