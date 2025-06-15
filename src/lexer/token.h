#pragma once
#ifndef TOKEN_H
#define TOKEN_H

#include <string>

#include "tokentype.h"

namespace Lexer {
    struct Token {
        TokenType type;
        std::string lexeme;
        int line;

        Token(TokenType type, std::string lexeme, int line) : type(type), lexeme(lexeme), line(line) {}
    };
}

#endif //TOKEN_H
