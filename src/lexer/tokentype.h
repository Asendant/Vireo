#pragma once

#ifndef TOKENTYPE_H
#define TOKENTYPE_H

#include <string>
#include <unordered_map>

namespace Lexer {
    enum class TokenType {
        // Keywords
        FUNCTION, IF, ELSE, ELSE_IF, WHILE, FOR, RETURN, VAR, // 0-7
        // Types
        TYPE_INT, TYPE_FLOAT, TYPE_STRING, TYPE_BOOL, // 8-11
        // Literals and Identifiers
        IDENTIFIER, INT_LITERAL, FLOAT_LITERAL, STRING_LITERAL, BOOL_LITERAL, // 12-16
        // Operators
        PLUS, MINUS, MUL, DIV, // 17-20
        EQUAL_TO, LESS, GREATER, // 21-23
        AND, OR, ASSIGN, //24-26
        // Punctuation
        LEFT_PAREN, RIGHT_PAREN, //27-28
        LEFT_BRACE, RIGHT_BRACE, //29-30
        COMMA, COLON, SEMICOLON, ARROW, //31-34
        // Meta
        END_OF_FILE, UNKNOWN // 35-36
    };
}

#endif
