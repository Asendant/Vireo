#pragma once
#include "lexer.h"
#include "tokentype.h"
#include <vector>

namespace Lexer {
    Lexer::Lexer(const std::string& code) {
        sourceCode = code;
        currentLine = 1;
        currentChar = 0;

        start = 0;
        current = 0;
    }
}
