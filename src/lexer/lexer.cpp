#include "lexer.h"
#include "token.h"
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

namespace Lexer {
    // Main Functions
    /**
     * @brief Creates the object for the lexer, reads the source code file and parses it into a string, and sets the starting and current positions.
     *
     * @param source The path to the file that contains the source code.
     * @param fromFile Has the lexer read the file from the file path if it is, or just use the source as the sourceCode
     */
    Lexer::Lexer(const std::string& source, bool fromFile) {
        if (fromFile) {
            sourceCode = ConvertSourceToString(source);
        } else {
            sourceCode = source;
        }
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

        while (!IsAtEnd()) {
            start = current;
            ScanToken();
        }

        tokens.emplace_back(TokenType::END_OF_FILE, "", currentLine);

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
            case '+': AddToken(TokenType::PLUS); break;
            case '-':
                if (Peek() == '>') {
                    Advance();
                    AddToken(TokenType::ARROW);
                } else {
                    AddToken(TokenType::MINUS);
                }
                break;
            case '*': AddToken(TokenType::MUL); break;
            case '/': AddToken(TokenType::DIV); break;
            case '=':
                if (Peek() == '=') {
                    Advance(); // Consume the second '='
                    AddToken(TokenType::EQUAL_TO);
                } else {
                    AddToken(TokenType::ASSIGN);
                }
                break;
            case '<': AddToken(TokenType::LESS); break;
            case '>': AddToken(TokenType::GREATER); break;
            case '&':
                if (Peek() == '&') {
                    Advance(); // Consume second '&'
                    AddToken(TokenType::AND);
                } else {
                    AddToken(TokenType::UNKNOWN);
                }
                break;
            case '|':
                if (Peek() == '|') {
                    Advance(); // Consume second '|'
                    AddToken(TokenType::OR);
                } else {
                    AddToken(TokenType::UNKNOWN);
                }
                break;
            case '(': AddToken(TokenType::LEFT_PAREN); break;
            case ')': AddToken(TokenType::RIGHT_PAREN); break;
            case '{': AddToken(TokenType::LEFT_BRACE); break;
            case '}': AddToken(TokenType::RIGHT_BRACE); break;
            case ',': AddToken(TokenType::COMMA); break;
            case ':': AddToken(TokenType::COLON); break;
            case ';': AddToken(TokenType::SEMICOLON); break;
            case '"': String(); break;
            case ' ':
            case '\t':
            case '\r':
            case '\n':
                break;
            default: {
                if (std::isdigit(static_cast<unsigned char>(c)))
                    Number();
                else if (std::isalpha(static_cast<unsigned char>(c)) || c == '_')
                    Identifier();
                else
                    AddToken(TokenType::UNKNOWN);
                break;
            }
        }
    }


    /**
     * @brief Advances the current character that the source code is on.
     *
     * @returns The character that it just passed.
    */
    char Lexer::Advance() {
        const char c = sourceCode[current++];  // Read current, then advance

        if (c == '\n') {
            currentLine++;
            currentChar = 0;
        } else {
            currentChar++;
        }

        return c;
    }

    /**
     * @brief Read digits in the source code to form an integer or float literal.
     */
    void Lexer::Number() {
        while (std::isdigit(Peek())) {
            Advance();
        }

        if (Peek() == '.' && std::isdigit(PeekNext())) {
            Advance();

            while (std::isdigit(Peek())) {
                Advance();
            }

            AddToken(TokenType::FLOAT_LITERAL);
        } else {
            AddToken(TokenType::INT_LITERAL);
        }
    }

    /**
     * @brief Read the source code as long as characters are letters, digits, or underscores and add a token identifier
    */
    void Lexer::Identifier() {
        while (std::isalnum(Peek()) || Peek() == '_') {
            Advance();
        }

        std::string identifier = sourceCode.substr(start, current - start);

        if (const auto tokenType = keywordMap.find(identifier); tokenType != keywordMap.end()) {
            AddToken(tokenType->second);
        } else {
            AddToken(TokenType::IDENTIFIER);
        }
    }

    /**
     * @brief Extracts a string literal from a pair of double quotes.
    */
    void Lexer::String() {
        while (Peek() != '"' && !IsAtEnd()) {
            Advance();
        }

        if (IsAtEnd()) {
            AddToken(TokenType::UNKNOWN);
            return;
        }

        Advance();

        const std::string value = sourceCode.substr(start + 1, current - start - 2);
        AddToken(TokenType::STRING_LITERAL, value);
    }

    /**
     * @brief Adds a token to the list of tokens that the lexer will pass to the parser.
     *
     * @param type The type of token to add to the list of tokens
    */
    void Lexer::AddToken(TokenType type, const std::string& lexeme) {
        std::string finalLexeme = lexeme.empty() ? sourceCode.substr(start, current - start) : lexeme;
        tokens.emplace_back(type, finalLexeme, currentLine);
    }

    /**
     * @brief Checks the current character.
     *
     * @returns The next character that the lexer will read.
     */
    char Lexer::Peek() const {
        if (current >= sourceCode.size()) return '\0';
        return sourceCode[current];
    }

    /**
     * @brief Shows the next character that is waiting in the queue.
     *
     * @return The next character in the queue.
    */
    char Lexer::PeekNext() const {
        if (current + 1 >= sourceCode.size()) return '\0';
        return sourceCode[current + 1];
    }

    /**
     * @brief Decides whether the program has reached the end of the source code.
     *
     * @returns Whether the program has reached the end of the source code.
    */
    bool Lexer::IsAtEnd() const {
        return current >= sourceCode.size();
    }

}
