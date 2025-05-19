#ifndef PARSER_H
#define PARSER_H

#include "token_type.h"
#include <vector>
#include <string>

class Parser {
public:
    Parser(const std::vector<Token>& tokens);

    void parse(); // Comienza el análisis
    std::vector<std::string> getErrors() const;

private:
    const std::vector<Token>& tokens;
    size_t current;
    std::vector<std::string> errors;

    // Funciones auxiliares
    std::string formatError(const Token& token, const std::string& message) const;
    bool match(Token::TokenType expectedType);
    bool isAtEnd() const;
    Token advance();
    Token peek() const;
    Token previous() const;

    void parseCommand();
};

#endif // PARSER_H
