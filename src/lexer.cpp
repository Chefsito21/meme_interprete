/**
 * @file lexer.cpp
 */

#include "lexer.h"
#include "token_type.h"
#include <fstream>
#include <cctype>
#include <iostream>

Lexer::Lexer(const AFD &afd) : afd_(afd), pos_(0), current_line_(1), current_column_(1) {}

int Lexer::get_symbol_index(const std::string &lexeme) const
{
    for (size_t i = 0; i < symbol_table_.size(); ++i)
    {
        if (symbol_table_[i].lexeme == lexeme)
            return static_cast<int>(i);
    }
    return -1;
}

char Lexer::current_char() const
{
    return pos_ < input_.size() ? input_[pos_] : '\0';
}

void Lexer::advance()
{
    if (current_char() == '\n')
    {
        current_line_++;
        current_column_ = 1;
    }
    else
    {
        current_column_++;
    }
    pos_++;
}

bool Lexer::is_in_symbol_table(const std::string &id) const
{
    for (const SymbolInfo &s : symbol_table_)
    {
        if (s.lexeme == id)
            return true;
    }
    return false;
}

void Lexer::add_token(Token::TokenType type, const std::string &lexeme)
{
    // Guardamos la posición inicial antes de avanzar
    int start_line = current_line_;
    int start_column = current_column_ - lexeme.length();
    if (start_column < 1)
        start_column = 1; // Asegurarnos que no sea negativo

    tokens_.push_back({type, lexeme, start_line, start_column});

    if ((type == Token::TokenType::ID ||
         type == Token::TokenType::COMANDO ||
         type == Token::TokenType::POSICION ||
         type == Token::TokenType::COLOR) &&
        !is_in_symbol_table(lexeme))
    {

        std::string specific_type = "-";
        if (type == Token::TokenType::COMANDO)
        {
            if (lexeme == "PON_IMAGEN")
                specific_type = "carga_imagen";
            else if (lexeme == "CAMBIA_TAMAÑO_TEXTO")
                specific_type = "tamaño_texto";
            else if (lexeme == "CAMBIA_COLOR_TEXTO")
                specific_type = "color_texto";
            else if (lexeme == "ESCRIBE")
                specific_type = "escribe_texto";
            else if (lexeme == "ESCRIBE_MULTILINEA")
                specific_type = "escribe_multilinea";
            else if (lexeme == "GUARDA_COMO")
                specific_type = "guardar_imagen";
        }

        symbol_table_.push_back({lexeme, Token::token_type_to_string(type), start_line, specific_type});
    }
}

void Lexer::process_token()
{
    if (isspace(current_char()))
    {
        advance();
        return;
    }

    std::string fragment = input_.substr(pos_);
    size_t length = 0;
    std::string match_type = afd_.run(fragment, length);
    std::string lexeme = fragment.substr(0, length);

    if (match_type == "ID")
        add_token(Token::determine_token_type(lexeme), lexeme);
    else if (match_type == "NUMERO")
        add_token(Token::TokenType::NUMERO, lexeme);
    else if (match_type == "CADENA")
        add_token(Token::determine_token_type(lexeme), lexeme);
    else if (lexeme == "//")
    {
        // Ignorar comentario de una línea
        while (current_char() != '\n' && current_char() != '\0')
            advance();
        advance(); // Avanzar sobre el '\n'
        return;
    }
    else if (lexeme == "/*")
    {
        // Ignorar comentario de varias líneas
        while (!(current_char() == '*' && pos_ + 1 < input_.size() && input_[pos_ + 1] == '/') && current_char() != '\0')
        {
            advance();
        }
        if (current_char() == '*')
        {
            advance();
            if (current_char() == '/')
            {
                advance();
            }
        }
        return;
    }
    else if (match_type == "COLOR_HEX")
        add_token(Token::TokenType::COLOR_HEX, lexeme);
    else if (match_type == "SIMBOLO")
        add_token(Token::TokenType::SIMBOLO, lexeme);
    else
        add_token(Token::TokenType::DESCONOCIDO, std::string(1, current_char()));

    // Avanzar por cada carácter del lexema
    for (size_t i = 0; i < length; ++i)
    {
        advance();
    }
}

std::vector<Token> Lexer::tokenize(const std::string &input)
{
    input_ = input;
    pos_ = 0;
    current_line_ = 1;
    current_column_ = 1;
    tokens_.clear();
    symbol_table_.clear();

    while (pos_ < input_.size())
    {
        process_token();
    }

    return tokens_;
}

void Lexer::export_symbol_table(const std::string &filename) const
{
    std::ofstream file(filename);
    if (!file)
        return;

    // Encabezado como comentario
    file << "# Formato: índice|lexema|tipo|línea|tipo_específico\n";

    for (size_t i = 0; i < symbol_table_.size(); ++i)
    {
        file << i << "|"
             << symbol_table_[i].lexeme << "|"
             << symbol_table_[i].type << "|"
             << symbol_table_[i].line << "|"
             << symbol_table_[i].specific_type << "\n";
    }
}