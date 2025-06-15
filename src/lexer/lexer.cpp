#pragma once
#include "lexer.h"
#include "token.h"
#include <vector>
#include <fstream>
#include <sstream>

namespace Lexer {
    // Main Functions
    /**
     * @brief Creates the object for the lexer, reads the source code file and parses it into a string, and sets the starting and current positions.
     *
     * @param filePath The path to the file that contains the source code.
     */
    Lexer::Lexer(const std::string& filePath) {
        sourceCode = ConvertSourceToString(filePath);
        currentLine = 1;
        currentChar = 0;

        start = 0;
        current = 0;
    }

    /**
     * @returns A list of tokens converted from the source code
     */
    std::vector<Token> Lexer::Tokenize() {
        start = 0;

        while (start < sourceCode.size()) {
            start = current;
            ScanToken();
        }

        tokens.emplace_back(TokenType::END_OF_FILE, "", 0);

        return tokens;
    }

    /**
     * @brief Scans the contents of file path and extracts them into a string.
     *
     * @param filePath The string that contains the path to the target source code.
     *
     * @returns The contents of the source code folder.
    **/
    // Helper Functions
    std::string Lexer::ConvertSourceToString(const std::string& filePath) {
        std::ifstream file(filePath);

        if (!file.is_open()) {
            return "";
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

    /**
     * @brief Scans the current token to check if it is a possible token.
    */
    void Lexer::ScanToken() {
        char c = Advance();
        switch (c) {
            case '\n': currentLine++; break;

            case ' ':
            case '\t':
            case '\r':
                break;

            default:
                if (std::isdigit(static_cast<unsigned char>(c)))
                    Number();
                else if (std::isalpha(static_cast<unsigned char>(c)) || c == '_')
                    Identifier();
                else
                    AddToken(TokenType::UNKNOWN);

        }
    }

    /**
     * @brief Advances the current character that the source code is on.
     *
     * @returns The character that it just passed.
     */
    char Lexer::Advance() {
        current++;
        return sourceCode[current - 1];
    }

    /**
     * @brief Read digits in the source code to form an integer or float literal.
     */
    void Lexer::Number() {

    }

    /**
     * @brief Read the source code as long as characters are letters, digits, or underscores and add a token identifier
    */
    void Lexer::Identifier() {

    }

    /**
     * @brief Adds a token to the list of tokens that the lexer will pass to the parser.
     *
     * @param type The type of token to add to the list of tokens
    */
    void Lexer::AddToken(TokenType type) {
        const std::string lexeme = sourceCode.substr(start, current - start);

        tokens.emplace_back(type, lexeme, current);
    }
}
