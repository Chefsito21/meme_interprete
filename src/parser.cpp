#include "parser.h"
#include "common.h"
#include <iostream>

Parser::Parser(const std::vector<Token> &tokens)
    : tokens(tokens), current(0) {}

void Parser::parse()
{
    std::cout << "Parsing..." << std::endl;
    while (!isAtEnd())
    {
        parseCommand();
    }

    if (!errors.empty())
    {
        for (const auto &err : errors)
        {
            std::cerr << err << std::endl;
        }
    }
}

void Parser::parseCommand()
{
    // ============================== Estructura de un comando ==============================
    if (!match(Token::TokenType::COMANDO))
    {
        Token t = peek();
        std::string msg = ERROR_COLOR BOLD "Error en línea " + std::to_string(t.line) +
                          ", columna " + std::to_string(t.column) + RESET ": Se esperaba un comando, pero se encontró '" +
                          t.lexeme + "'";
        errors.push_back(msg);
        advance();
        return;
    }

    Token comando = previous();

    if (!match(Token::TokenType::SIMBOLO) || previous().lexeme != "(")
    {
        Token t = previous();
        std::string msg = ERROR_COLOR BOLD "Error en línea " + std::to_string(t.line) +
                          ", columna " + std::to_string(t.column) + RESET ": Se esperaba '('" +
                          " después del comando '" + comando.lexeme + "'";
        errors.push_back(msg);
        return;
    }

    // ============================== Validación de argumentos ==============================
    if (comando.lexeme == "PON_IMAGEN" || comando.lexeme == "GUARDA_COMO")
    {
        if (!match(Token::TokenType::CADENA))
        {
            Token t = peek();
            errors.push_back(formatError(t, "Se esperaba una cadena con la ruta del archivo."));
        } else {
            Token argumento = previous();

            if (comando.lexeme == "PON_IMAGEN") {
                ast.push_back(std::make_shared<PonerImagen>(argumento.lexeme));
            } else {
                ast.push_back(std::make_shared<GuardaComo>(argumento.lexeme));
            }                            
        }
    }
    else if (comando.lexeme == "CAMBIA_COLOR_TEXTO")
    {

        if (!match(Token::TokenType::COLOR) && !match(Token::TokenType::COLOR_HEX))
        {
            Token t = peek();
            errors.push_back(formatError(t, "Se esperaba un color válido (como 'ROJO' o '#FFFFFF')."));
        } else {
            Token arg = previous();
            ast.push_back(std::make_shared<CambiarColorTexto>(arg.lexeme));
        }
    }
    else if (comando.lexeme == "CAMBIA_TAMAÑO_TEXTO")
    {
        if (!match(Token::TokenType::NUMERO))
        {
            Token t = peek();
            errors.push_back(formatError(t, "Se esperaba un número para el tamaño del texto (10–30)."));
        }
        else
        {
            Token numero = previous();
            int size = std::stoi(numero.lexeme);
            if (size < 10 || size > 30)
            {
                errors.push_back(formatError(numero, "El tamaño del texto debe estar entre 10 y 30."));
            } else {
                ast.push_back(std::make_shared<CambiarTamanoTexto>(size));
            }
        }
    }
    else if (comando.lexeme == "ESCRIBE")
    {
        std::string texto, pos;

        if (!match(Token::TokenType::CADENA))
        {
            Token t = peek();
            errors.push_back(formatError(t, "Se esperaba una cadena de texto como primer argumento."));
        } else {
            texto = previous().lexeme;
        }

        if (!match(Token::TokenType::SIMBOLO) || previous().lexeme != ",")
        {
            Token t = previous();
            errors.push_back(formatError(t, "Se esperaba ',' entre los argumentos de ESCRIBE."));
        }

        if (!match(Token::TokenType::POSICION))
        {
            Token t = peek();
            errors.push_back(formatError(t, "Se esperaba una posición válida (ARRIBA, ABAJO, etc.) como segundo argumento."));
        } else {
            pos = previous().lexeme;
        }

        if (errors.empty()) {
            ast.push_back(std::make_shared<Escribe>(texto, pos));
        }
    } else if (comando.lexeme == "ESCRIBE_MULTILINEA") {
        std::string t1, t2, pos;
        // Primer argumento: cadena
        if (!match(Token::TokenType::CADENA)) {
            Token t = peek();
            errors.push_back(formatError(t, "Se esperaba una primera cadena de texto."));
        } else {
            t1 = previous().lexeme;
        }
    
        // Coma
        if (!match(Token::TokenType::SIMBOLO) || previous().lexeme != ",") {
            Token t = previous();
            errors.push_back(formatError(t, "Se esperaba ',' después de la primera cadena."));
        }
    
        // Segundo argumento: cadena
        if (!match(Token::TokenType::CADENA)) {
            Token t = peek();
            errors.push_back(formatError(t, "Se esperaba una segunda cadena de texto."));
        } else {
            t2 = previous().lexeme;
        }
    
        // Coma
        if (!match(Token::TokenType::SIMBOLO) || previous().lexeme != ",") {
            Token t = previous();
            errors.push_back(formatError(t, "Se esperaba ',' después de la segunda cadena."));
        }
    
        // Tercer argumento: posición
        if (!match(Token::TokenType::POSICION)) {
            Token t = peek();
            errors.push_back(formatError(t, "Se esperaba una posición válida (ARRIBA, ABAJO, etc.) como tercer argumento."));
        } else {
            pos = previous().lexeme;
        }

        if (errors.empty()) {
            ast.push_back(std::make_shared<EscribeMultilinea>(t1, t2, pos));
        }
    } else if (comando.lexeme == "GUARDA_COMO") {
        if (!match(Token::TokenType::CADENA)) {
            errors.push_back(formatError(peek(), "Se esperaba una ruta entre comillas."));
        } else {
            Token arg = previous();
            ast.push_back(std::make_shared<GuardaComo>(arg.lexeme));
        }
    }
    
    

    while (!isAtEnd() && peek().lexeme != ")")
    {
        advance(); 
    }

    if (!match(Token::TokenType::SIMBOLO) || previous().lexeme != ")")
    {
        Token t = previous();
        std::string msg = ERROR_COLOR BOLD "Error en línea " + std::to_string(t.line) +
                          ", columna " + std::to_string(t.column) + RESET ": Se esperaba ')'" +
                          " para cerrar el comando '" + comando.lexeme + "'";
        errors.push_back(msg);
    }
}

bool Parser::match(Token::TokenType expectedType)
{
    if (isAtEnd())
        return false;
    if (peek().type != expectedType)
        return false;
    advance();
    return true;
}

Token Parser::advance()
{
    if (!isAtEnd())
        current++;
    return previous();
}

bool Parser::isAtEnd() const
{
    return current >= tokens.size();
}

Token Parser::peek() const
{
    return tokens[current];
}

Token Parser::previous() const
{
    return tokens[current - 1];
}

std::vector<std::string> Parser::getErrors() const
{
    return errors;
}

std::string Parser::formatError(const Token &t, const std::string &message) const
{
    return ERROR_COLOR BOLD "Error en línea " + std::to_string(t.line) +
           ", columna " + std::to_string(t.column) + RESET ": " + message;
}

std::vector<std::shared_ptr<ASTNode>> Parser::getAST() const {
    return ast;
}