// token.cpp
#include "token_type.h"
#include <unordered_set>
#include <iostream>

std::string Token::token_type_to_string(TokenType type) {
    switch (type) {
        case TokenType::ID: return "ID";
        case TokenType::NUMERO: return "NUMERO";
        case TokenType::CADENA: return "CADENA";
        case TokenType::SIMBOLO: return "SIMBOLO";
        case TokenType::COLOR_HEX: return "COLOR_HEX";
        case TokenType::COMANDO: return "COMANDO";
        case TokenType::POSICION: return "POSICION";
        case TokenType::COLOR: return "COLOR";
        case TokenType::DESCONOCIDO: 
        default: return "DESCONOCIDO";
    }
}

Token::TokenType Token::determine_token_type(const std::string& lexeme) {
    static const std::unordered_set<std::string> comandos = {
        "PON_IMAGEN", "CAMBIA_COLOR_TEXTO", "CAMBIA_TAMAÑO_TEXTO",
        "ESCRIBE", "ESCRIBE_MULTILINEA", "GUARDA_COMO"
    };

    static const std::unordered_set<std::string> posiciones = {
        "ARRIBA", "ABAJO", "CENTRO", "IZQUIERDA_SUPERIOR", "DERECHA_INFERIOR"
    };

    static const std::unordered_set<std::string> colores = {
        "BLANCO", "NEGRO", "ROJO", "AZUL", "VERDE", "AMARILLO", "MAGENTA", "CYAN"
    };

    // Verificar si es una cadena entre comillas
    if (lexeme.size() >= 2 && lexeme.front() == '"' && lexeme.back() == '"') {
        // Extraer el contenido sin las comillas
        std::string contenido = lexeme.substr(1, lexeme.size() - 2);
        
        // Verificar si es un color
        if (colores.find(contenido) != colores.end()) {
            return TokenType::COLOR;
        }
        
        // Verificar si es un código hexadecimal de color
        if (contenido.size() == 7 && contenido[0] == '#' && 
            std::all_of(contenido.begin()+1, contenido.end(), [](char c) {
                return isxdigit(c);
            })) {
            return TokenType::COLOR_HEX;
        }
        
        // Si no es color, es una cadena normal
        return TokenType::CADENA;
    }
    
    // Verificar si es un número (sin comillas)
    bool is_number = !lexeme.empty();
    for (char c : lexeme) {
        if (!isdigit(c)) {
            is_number = false;
            break;
        }
    }
    if (is_number) {
        return TokenType::NUMERO;
    }

    // Verificar otras palabras reservadas (sin comillas)
    if (comandos.find(lexeme) != comandos.end()) {
        return TokenType::COMANDO;
    } else if (posiciones.find(lexeme) != posiciones.end()) {
        return TokenType::POSICION;
    }
    
    // Si no coincide con nada anterior, es un ID
    return TokenType::ID;
}