#ifndef TOKEN_TYPE_H
#define TOKEN_TYPE_H

#include <string>
#include <unordered_set>

class Token {
public:
    enum class TokenType {
        ID,
        COMANDO,
        POSICION,
        COLOR,
        NUMERO,
        CADENA,
        SIMBOLO,
        COLOR_HEX,
        DESCONOCIDO
    };

    TokenType type;
    std::string lexeme;
    int line;    // Línea donde empieza el token
    int column;  // Columna donde empieza el token

    Token(TokenType type, const std::string& lexeme, int line = 0, int column = 0)
        : type(type), lexeme(lexeme), line(line), column(column) {}

    static std::string token_type_to_string(TokenType type);
    static TokenType determine_token_type(const std::string& lexeme);
};

#endif // TOKEN_TYPE_H