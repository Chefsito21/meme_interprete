#ifndef LEXER_H
#define LEXER_H

#include "afd.h"
#include "token_type.h"
#include <vector>
#include <string>

struct SymbolInfo {
    std::string lexeme;
    std::string type;
    int line;
    std::string specific_type;  // Nuevo campo para el tipo específico
};

class Lexer {
public:
    Lexer(const AFD& afd);
    std::vector<Token> tokenize(const std::string& input);
    void export_symbol_table(const std::string& filename) const;
    int get_symbol_index(const std::string& lexeme) const;

private:
    const AFD& afd_;
    size_t pos_;
    std::string input_;
    int current_line_;      // Línea actual en el análisis
    int current_column_;    // Columna actual en el análisis

    char current_char() const;
    void advance();
    void add_token(Token::TokenType type, const std::string& lexeme);
    void process_token();
    bool is_in_symbol_table(const std::string& id) const;

    std::vector<Token> tokens_;
    std::vector<SymbolInfo> symbol_table_;
};

#endif // LEXER_H