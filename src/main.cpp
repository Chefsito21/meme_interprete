#include <iostream>
#include <fstream>
#include <sstream>
#include "afd.h"
#include "lexer.h"
#include "token_type.h"
#include "parser.h"
#include "optimizer.h"
// #include "ast_visualizer.h"



int main(int argc, char* argv[]) {
    if (argc < 2) {
    std::cerr << "Uso: " << argv[0] << " archivo.meme" << std::endl;
    return 1;
    }
    std::string input_filename = argv[1];
    if (input_filename.substr(input_filename.find_last_of('.')) != ".meme") {
        std::cerr << "Error: El archivo debe tener extensión .meme" << std::endl;
        return 1;
    }
    
    // Leer el archivo fuente
    std::ifstream input_file(input_filename);
    if (!input_file) {
        std::cerr << "Error al abrir el archivo: " << input_filename << std::endl;
        return 1;
    }
    
    std::stringstream buffer;
    buffer << input_file.rdbuf();
    std::string source = buffer.str();
    
    // Cargar el AFD
    AFD afd;
    try {
        afd.load_from_file("../include/AFDs/afd_simbolos.json");
    } catch (const std::exception& e) {
        std::cerr << "Error al cargar el AFD: " << e.what() << std::endl;
        return 1;
    }
    
    // Ejecutar el lexer
    Lexer lexer(afd);
    std::vector<Token> tokens = lexer.tokenize(source);
    
    // Crear nombres de archivo de salida
    // std::string base_name = input_filename.substr(0, input_filename.find_last_of('.'));
    std::string output_filename = "../samples/tokens.out";
    std::string symbols_filename = "../samples/tabla_simbolos.txt";
    
    // Guardar tokens formateados
    std::ofstream out_file(output_filename);
    if (!out_file) {
        std::cerr << "Error al escribir en " << output_filename << std::endl;
        return 1;
    }
    // Imprimir tokens
    std::cout << std::endl;
    std::cout << "Tokens generados:\n";
    for (const auto& token: tokens) {
        std::cout << "⟨" << Token::token_type_to_string(token.type) << ", " << token.lexeme << "⟩ ";
    }
    std::cout << std::endl;
    std::cout << std::endl;
    
    for (const auto& token : tokens) {
        switch (token.type) {
            case Token::TokenType::COMANDO:
            case Token::TokenType::POSICION:
            case Token::TokenType::COLOR:
            case Token::TokenType::COLOR_HEX: {
                int index = lexer.get_symbol_index(token.lexeme);
                out_file << "⟨" << Token::token_type_to_string(token.type) << ", " << index << "⟩ ";
                break;
            }
            
            case Token::TokenType::NUMERO: {
                out_file << "⟨num, " << token.lexeme << "⟩ ";
                break;
            }
            
            case Token::TokenType::CADENA: {
                // Eliminar comillas adicionales y escapar caracteres especiales
                std::string clean_lexeme = token.lexeme;
                if (clean_lexeme.front() == '"' && clean_lexeme.back() == '"') {
                    clean_lexeme = clean_lexeme.substr(1, clean_lexeme.length() - 2);
                }
                // Escapar comillas internas si es necesario
                std::string escaped;
                for (char c : clean_lexeme) {
                    if (c == '"') escaped += "\\\"";
                    else escaped += c;
                }
                out_file << "⟨const, \"" << escaped << "\"⟩ ";
                break;
            }
            
            case Token::TokenType::SIMBOLO: {
                if (token.lexeme == "(") out_file << "⟨(⟩ ";
                else if (token.lexeme == ")") out_file << "⟨)⟩ ";
                else if (token.lexeme == ",") out_file << "⟨,⟩ ";
                else out_file << "⟨" << token.lexeme << "⟩ ";
                break;
            }
            
            case Token::TokenType::DESCONOCIDO: {
                out_file << "⟨unknown, '" << token.lexeme << "'⟩ ";
                break;
            }
            
            default: {
                out_file << "⟨" << Token::token_type_to_string(token.type) << ", " << token.lexeme << "⟩ ";
                break;
            }
        }
    }
    
    
    out_file << std::endl;
    out_file.close();
    
    // Guardar tabla de símbolos
    lexer.export_symbol_table(symbols_filename);
    
    std::cout << "Análisis completo. Tokens en: " << output_filename << "\nTabla de símbolos en: " << symbols_filename << std::endl;
    


    // Parser
    Parser parser(tokens);
    parser.parse();
    if (!parser.getErrors().empty()) return 1;

    Optimizer opt;
    opt.optimize(parser.ast, "../samples/programa_optimizado.out");

    int status = std::system("python3 ../tools/image_interpreter.py");
    if (status != 0) {
        std::cerr << "Error al ejecutar el intérprete de imágenes." << std::endl;
    }

    // if (parser.getErrors().empty()) {
    //     ASTVisualizer visualizer;
    //     visualizer.exportToDot(parser.ast, "ast.dot");
    //     std::cout << "AST exportado a ast.dot\n";
    // } else {
    //     std::cerr << "Errores encontrados. AST no exportado.\n";
    // }

    return 0;
}