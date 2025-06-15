#pragma once
#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <utility>

#include "tokentype.h"

namespace Lexer {
    struct Token {
        TokenType type;
        std::string lexeme;
        int line;

        Token(const TokenType type, std::string  lexeme, const int line) : type(type), lexeme(std::move(lexeme)), line(line) {}
    };
}

#endif //TOKEN_H
