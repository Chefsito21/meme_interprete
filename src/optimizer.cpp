#include "optimizer.h"
#include <fstream>
#include <iostream>

void Optimizer::optimize(const std::vector<std::shared_ptr<ASTNode>>& ast, const std::string& outputPath) {
    std::ofstream out(outputPath);
    if (!out.is_open()) {
        std::cerr << "No se pudo abrir el archivo de salida: " << outputPath << std::endl;
        return;
    }

    for (const auto& cmd : ast) {
        if (auto c = std::dynamic_pointer_cast<CambiarColorTexto>(cmd)) {
            std::string color = c->color;
            if (color == "BLANCO") color = "#FFFFFF";
            else if (color == "\"NEGRO\"") color = "#000000";
            else if (color == "\"ROJO\"") color = "#FF0000";
            else if (color == "\"AZUL\"") color = "#0000FF";
            else if (color == "\"VERDE\"") color = "#00FF00";
            out << "CAMBIA_COLOR_TEXTO(" << color << ")\n";
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
