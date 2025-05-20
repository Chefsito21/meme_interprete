#include "optimizer.h"
#include <fstream>
#include <iostream>
#include <unordered_map>

void Optimizer::optimize(const std::vector<std::shared_ptr<ASTNode>>& ast, const std::string& outputPath) {
    std::ofstream out(outputPath);
    if (!out.is_open()) {
        std::cerr << "No se pudo abrir el archivo de salida: " << outputPath << std::endl;
        return;
    }

    // Mapeo de colores por nombre
    static const std::unordered_map<std::string, std::string> colorMap = {
        {"\"NEGRO\"", "#000000"},
        {"\"BLANCO\"", "#FFFFFF"},
        {"\"ROJO\"", "#FF0000"},
        {"\"VERDE\"", "#00FF00"},
        {"\"AZUL\"", "#0000FF"},
        {"\"AMARILLO\"", "#FFFF00"},
        {"\"CIAN\"", "#00FFFF"},
        {"\"MAGENTA\"", "#FF00FF"},
        {"\"GRIS\"", "#808080"},
        {"\"GRIS_CLARO\"", "#D3D3D3"},
        {"\"NARANJA\"", "#FFA500"},
        {"\"ROSADO\"", "#FFC0CB"},
        {"\"LIMA\"", "#BFFF00"},
        {"\"VIOLETA\"", "#8A2BE2"},
        {"\"MARRON\"", "#8B4513"},
        {"\"OLIVA\"", "#808000"},
        {"\"TURQUESA\"", "#40E0D0"},
        {"\"DORADO\"", "#FFD700"},
        {"\"PLATA\"", "#C0C0C0"},
        {"\"AGUAMARINA\"", "#7FFFD4"},
        {"\"INDIGO\"", "#4B0082"},
        {"\"PURPURA\"", "#800080"},
        {"\"BEIGE\"", "#F5F5DC"},
        {"\"MANTEQUILLA\"", "#FFE4B5"},
        
    };

    for (const auto& cmd : ast) {
        if (auto c = std::dynamic_pointer_cast<CambiarColorTexto>(cmd)) {
            std::string color = c->color;
            auto it = colorMap.find(color);
            if (it != colorMap.end()) {
                out << "CAMBIA_COLOR_TEXTO(" << it->second << ")\n";
            } else {
                std::cerr << "Color no reconocido: " << color << std::endl;
            }
        }
        else if (auto c = std::dynamic_pointer_cast<CambiarTamanoTexto>(cmd)) {
            out << "CAMBIA_TAMAÑO_TEXTO(" << c->tamano << ")\n";
        }
        else if (auto c = std::dynamic_pointer_cast<PonerImagen>(cmd)) {
            out << "PON_IMAGEN(" << c->ruta << ")\n";
        }
        else if (auto c = std::dynamic_pointer_cast<Escribe>(cmd)) {
            out << "ESCRIBE(" << c->texto << ", " << c->posicion << ")\n";
        }
        else if (auto c = std::dynamic_pointer_cast<EscribeMultilinea>(cmd)) {
            out << "ESCRIBE_MULTILINEA(" << c->texto1 << ", " << c->texto2 << ", " << c->posicion << ")\n";
        }
        else if (auto c = std::dynamic_pointer_cast<GuardaComo>(cmd)) {
            out << "GUARDA_COMO(" << c->ruta << ")\n";
        }
    }

    out.close();
}