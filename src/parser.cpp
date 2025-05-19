#include "parser.h"
#include "common.h"
#include <iostream>

Parser::Parser(const std::vector<Token>& tokens)
    : tokens(tokens), current(0) {}

void Parser::parse() {
    std::cout << "Parsing..." << std::endl;
    while (!isAtEnd()) {
        parseCommand();
    }

    if (!errors.empty()) {
        for (const auto& err : errors) {
            std::cerr << err << std::endl;
        }
    }
}

void Parser::parseCommand() {
    // ============================== Estructura de un comando ==============================
    if (!match(Token::TokenType::COMANDO)) {
        Token t = peek();
        std::string msg = ERROR_COLOR BOLD "Error en línea " + std::to_string(t.line) +
                          ", columna " + std::to_string(t.column) + RESET ": Se esperaba un comando, pero se encontró '" +
                          t.lexeme + "'";
        errors.push_back(msg);
        advance();
        return;
    }

    Token comando = previous();

    if (!match(Token::TokenType::SIMBOLO) || previous().lexeme != "(") {
        Token t = previous();
        std::string msg = ERROR_COLOR BOLD "Error en línea " + std::to_string(t.line) +
                          ", columna " + std::to_string(t.column) + RESET ": Se esperaba '('" +
                          " después del comando '" + comando.lexeme + "'";
        errors.push_back(msg);
        return;
    }

    // ============================== Validación de argumentos ==============================
    if (comando.lexeme == "PON_IMAGEN" || comando.lexeme == "GUARDA_COMO") {
        if (!match(Token::TokenType::CADENA)) {
            Token t = peek();
            errors.push_back(formatError(t, "Se esperaba una cadena con la ruta del archivo."));
        }
    }
    else if (comando.lexeme == "CAMBIA_COLOR_TEXTO") {
        
        if (!match(Token::TokenType::COLOR) && !match(Token::TokenType::COLOR_HEX)) {
            Token t = peek();
            errors.push_back(formatError(t, "Se esperaba un color válido (como 'ROJO' o '#FFFFFF')."));
        }
    }
    while (!isAtEnd() && peek().lexeme != ")") {
        advance();  // Aquí luego validaremos los argumentos
    }

    if (!match(Token::TokenType::SIMBOLO) || previous().lexeme != ")") {
        Token t = previous();
        std::string msg = ERROR_COLOR BOLD "Error en línea " + std::to_string(t.line) +
                          ", columna " + std::to_string(t.column) + RESET ": Se esperaba ')'" +
                          " para cerrar el comando '" + comando.lexeme + "'";
        errors.push_back(msg);
    }
}


bool Parser::match(Token::TokenType expectedType) {
    if (isAtEnd()) return false;
    if (peek().type != expectedType) return false;
    advance();
    return true;
}

Token Parser::advance() {
    if (!isAtEnd()) current++;
    return previous();
}

bool Parser::isAtEnd() const {
    return current >= tokens.size();
}

Token Parser::peek() const {
    return tokens[current];
}

Token Parser::previous() const {
    return tokens[current - 1];
}

std::vector<std::string> Parser::getErrors() const {
    return errors;
}

std::string Parser::formatError(const Token& t, const std::string& message) const {
    return ERROR_COLOR BOLD "Error en línea " + std::to_string(t.line) +
           ", columna " + std::to_string(t.column) + RESET ": " + message;
}
