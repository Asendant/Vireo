#pragma once

#ifndef TOKENTYPE_H
#define TOKENTYPE_H
namespace Lexer {
    enum class TokenType {
        // Keywords
        FUNCTION, IF, ELSE, ELSE_IF, WHILE, FOR, RETURN, VAR,
        // Types
        TYPE_INT, TYPE_FLOAT, TYPE_STRING, TYPE_BOOl,
        // Literals and Identifiers
        IDENTIFIER, INT_LITERAL, FLOAT_LITERAL, STRING_LITERAL, BOOL_LITERAL,
        // Operators
        PLUS, MINUS, MUL, DIV,
        EQUAL_TO, LESS, GREATER,
        AND, OR, ASSIGN,
        // Punctuation
        LEFT_PAREN, RIGHT_PAREN,
        LEFT_BRACE, RIGHT_BRACE,
        COMMA, COLON, SEMICOLON, ARROW,
        // Meta
        END_OF_FILE, UNKNOWN
    };
}

#endif
