#define CATCH_CONFIG_RUNNER
#include "../lib/catch.hpp"
#include "../src/lexer/lexer.h"
#include "../src/lexer/token.h"
#include "../src/lexer/tokentype.h"

int main(int argc, char* argv[]) {
    Catch::Session session;

    if (argc == 1) {
        // CLion or CTest didn't pass any args — inject our own
        const char* newArgs[] = {
            argv[0],
            "-r", "console",
            "-d", "yes",
            "--order", "lex"
        };
        int newArgc = sizeof(newArgs) / sizeof(newArgs[0]);
        return session.run(newArgc, const_cast<char**>(newArgs));
    }

    // CTest or CLion passed args — use as-is
    return session.run(argc, argv);
}

TEST_CASE("Lexer handles basic arithmetic tokens", "[lexer]") {
    std::string input = "+ - * /";
    Lexer::Lexer lexer(input, false);
    auto tokens = lexer.Tokenize();

    REQUIRE(tokens[0].type == Lexer::TokenType::PLUS);

    REQUIRE(tokens[1].type == Lexer::TokenType::MINUS);

    REQUIRE(tokens[2].type == Lexer::TokenType::MUL);

    REQUIRE(tokens[3].type == Lexer::TokenType::DIV);

    REQUIRE(tokens.back().type == Lexer::TokenType::END_OF_FILE);
}

TEST_CASE("Lexer handles all token types", "[lexer][all_tokens]") {
    std::string input =
        "function if else else_if while for return var " // 0-7
        "int float string bool " // 8-11
        "123 45.67 \"hello\" true false " // 12-16
        "+ - * / == < > && || = " // 17-26
        "( ) { } , : ; -> " //27-34
        "identifier1 _varName"; //35-36

    Lexer::Lexer lexer(input, false);
    auto tokens = lexer.Tokenize();

    std::vector<Lexer::TokenType> expected = {
        // Keywords
        Lexer::TokenType::FUNCTION, Lexer::TokenType::IF, Lexer::TokenType::ELSE,
        Lexer::TokenType::ELSE_IF, Lexer::TokenType::WHILE, Lexer::TokenType::FOR,
        Lexer::TokenType::RETURN, Lexer::TokenType::VAR,

        // Types
        Lexer::TokenType::TYPE_INT, Lexer::TokenType::TYPE_FLOAT,
        Lexer::TokenType::TYPE_STRING, Lexer::TokenType::TYPE_BOOL,

        // Literals
        Lexer::TokenType::INT_LITERAL, Lexer::TokenType::FLOAT_LITERAL,
        Lexer::TokenType::STRING_LITERAL, Lexer::TokenType::BOOL_LITERAL, Lexer::TokenType::BOOL_LITERAL, // true and false are identifiers here unless explicitly mapped

        // Operators
        Lexer::TokenType::PLUS, Lexer::TokenType::MINUS, Lexer::TokenType::MUL, Lexer::TokenType::DIV,
        Lexer::TokenType::EQUAL_TO, Lexer::TokenType::LESS, Lexer::TokenType::GREATER,
        Lexer::TokenType::AND, Lexer::TokenType::OR, Lexer::TokenType::ASSIGN,

        // Punctuation
        Lexer::TokenType::LEFT_PAREN, Lexer::TokenType::RIGHT_PAREN,
        Lexer::TokenType::LEFT_BRACE, Lexer::TokenType::RIGHT_BRACE,
        Lexer::TokenType::COMMA, Lexer::TokenType::COLON,
        Lexer::TokenType::SEMICOLON, Lexer::TokenType::ARROW,

        // Identifiers
        Lexer::TokenType::IDENTIFIER, Lexer::TokenType::IDENTIFIER,

        // EOF
        Lexer::TokenType::END_OF_FILE
    };

    for (int i = 0; i < std::min(tokens.size(), expected.size()); ++i) {
        INFO("At token index " << i << ": expected " << static_cast<int>(expected[i])
            << ", got " << static_cast<int>(tokens[i].type));
        REQUIRE(tokens[i].type == expected[i]
    );
}

}

TEST_CASE("Lexer handles unknown tokens", "[lexer][error]") {
    std::string input = "@ # $ % ^";
    Lexer::Lexer lexer(input, false);
    auto tokens = lexer.Tokenize();

    for (size_t i = 0; i < tokens.size() - 1; ++i) {
        REQUIRE(tokens[i].type == Lexer::TokenType::UNKNOWN);
    }

    REQUIRE(tokens.back().type == Lexer::TokenType::END_OF_FILE);
}

TEST_CASE("Lexer handles unterminated string literal", "[lexer][string]") {
    std::string input = "\"this is not closed";
    Lexer::Lexer lexer(input, false);
    auto tokens = lexer.Tokenize();

    REQUIRE(tokens[0].type == Lexer::TokenType::UNKNOWN); // should fail gracefully
    REQUIRE(tokens.back().type == Lexer::TokenType::END_OF_FILE);
}

TEST_CASE("Lexer skips whitespace correctly", "[lexer][whitespace]") {
    std::string input = "   \n\t  var   \n  ";
    Lexer::Lexer lexer(input, false);
    auto tokens = lexer.Tokenize();

    REQUIRE(tokens[0].type == Lexer::TokenType::VAR);
    REQUIRE(tokens[1].type == Lexer::TokenType::END_OF_FILE);
}

TEST_CASE("Lexer distinguishes identifiers from keywords", "[lexer][identifiers]") {
    std::string input = "if iffy var variable";
    Lexer::Lexer lexer(input, false);
    auto tokens = lexer.Tokenize();

    REQUIRE(tokens[0].type == Lexer::TokenType::IF);
    REQUIRE(tokens[1].type == Lexer::TokenType::IDENTIFIER);
    REQUIRE(tokens[2].type == Lexer::TokenType::VAR);
    REQUIRE(tokens[3].type == Lexer::TokenType::IDENTIFIER);
    REQUIRE(tokens.back().type == Lexer::TokenType::END_OF_FILE);
}

TEST_CASE("Lexer parses a small program correctly", "[lexer][demo]") {
    std::string input =
        "function add -> int {\n"
        "var x: int = 10;\n"
        "var y: int = 20;\n"
        "return x + y;\n"
        "}";

    Lexer::Lexer lexer(input, false);
    auto tokens = lexer.Tokenize();

    std::vector<Lexer::TokenType> expected = {
        Lexer::TokenType::FUNCTION,
        Lexer::TokenType::IDENTIFIER,
        Lexer::TokenType::ARROW,
        Lexer::TokenType::TYPE_INT,
        Lexer::TokenType::LEFT_BRACE,

        Lexer::TokenType::VAR,
        Lexer::TokenType::IDENTIFIER,
        Lexer::TokenType::COLON,
        Lexer::TokenType::TYPE_INT,
        Lexer::TokenType::ASSIGN,
        Lexer::TokenType::INT_LITERAL,
        Lexer::TokenType::SEMICOLON,

        Lexer::TokenType::VAR,
        Lexer::TokenType::IDENTIFIER,
        Lexer::TokenType::COLON,
        Lexer::TokenType::TYPE_INT,
        Lexer::TokenType::ASSIGN,
        Lexer::TokenType::INT_LITERAL,
        Lexer::TokenType::SEMICOLON,

        Lexer::TokenType::RETURN,
        Lexer::TokenType::IDENTIFIER,
        Lexer::TokenType::PLUS,
        Lexer::TokenType::IDENTIFIER,
        Lexer::TokenType::SEMICOLON,

        Lexer::TokenType::RIGHT_BRACE,
        Lexer::TokenType::END_OF_FILE
    };

    REQUIRE(tokens.size() == expected.size());

    for (size_t i = 0; i < expected.size(); ++i) {
        INFO("At token index " << i << ": expected " << static_cast<int>(expected[i])
             << ", got " << static_cast<int>(tokens[i].type));
        REQUIRE(tokens[i].type == expected[i]);
    }
}