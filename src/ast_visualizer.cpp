#include "ast_visualizer.h"
#include <fstream>
#include <iostream>

static int nodeCounter = 0;

std::string escapeDotString(const std::string& input) {
    std::string output;
    for (char c : input) {
        switch (c) {
            case '"': output += "\\\""; break;
            case '\\': output += "\\\\"; break;
            case '\n': output += "\\n"; break;
            case '\r': output += "\\r"; break;
            case '\t': output += "\\t"; break;
            default: output += c;
        }
    }
    return output;
}

std::string createNode(std::ofstream& out, const std::string& label) {
    std::string nodeName = "node" + std::to_string(nodeCounter++);
    out << "    " << nodeName << " [label=\"" << escapeDotString(label) << "\"];\n";
    return nodeName;
}

void ASTVisualizer::exportToDot(const std::vector<std::shared_ptr<ASTNode>>& ast, const std::string& filename) {
    std::ofstream out(filename);
    if (!out.is_open()) {
        std::cerr << "No se pudo abrir el archivo: " << filename << std::endl;
        return;
    }

    out << "digraph AST {\n";
    out << "    node [shape=box, style=filled, fillcolor=lightgray];\n";
    out << "    rankdir=TB;\n";

    std::string root = createNode(out, "Programa");

    for (const auto& cmd : ast) {
        std::string cmdNode;

        if (auto c = std::dynamic_pointer_cast<PonerImagen>(cmd)) {
            cmdNode = createNode(out, "PON_IMAGEN");
            std::string rutaNode = createNode(out, "\"" + c->ruta + "\"");
            out << "    " << cmdNode << " -> " << rutaNode << ";\n";
        } else if (auto c = std::dynamic_pointer_cast<CambiarColorTexto>(cmd)) {
            cmdNode = createNode(out, "CAMBIA_COLOR_TEXTO");
            std::string colorNode = createNode(out, c->color);
            out << "    " << cmdNode << " -> " << colorNode << ";\n";
        } else if (auto c = std::dynamic_pointer_cast<CambiarTamanoTexto>(cmd)) {
            cmdNode = createNode(out, "CAMBIA_TAMANO_TEXTO");
            std::string sizeNode = createNode(out, std::to_string(c->tamano));
            out << "    " << cmdNode << " -> " << sizeNode << ";\n";
        } else if (auto c = std::dynamic_pointer_cast<Escribe>(cmd)) {
            cmdNode = createNode(out, "ESCRIBE");
            std::string txtNode = createNode(out, c->texto );
            std::string posNode = createNode(out, c->posicion);
            out << "    " << cmdNode << " -> " << txtNode << ";\n";
            out << "    " << cmdNode << " -> " << posNode << ";\n";
        } else if (auto c = std::dynamic_pointer_cast<EscribeMultilinea>(cmd)) {
            cmdNode = createNode(out, "ESCRIBE_MULTILINEA");
            std::string l1 = createNode(out, c->texto1 );
            std::string l2 = createNode(out, c->texto2 );
            std::string pos = createNode(out, c->posicion);
            out << "    " << cmdNode << " -> " << l1 << ";\n";
            out << "    " << cmdNode << " -> " << l2 << ";\n";
            out << "    " << cmdNode << " -> " << pos << ";\n";
        } else if (auto c = std::dynamic_pointer_cast<GuardaComo>(cmd)) {
            cmdNode = createNode(out, "GUARDA_COMO");
            std::string pathNode = createNode(out, c->ruta);
            out << "    " << cmdNode << " -> " << pathNode << ";\n";
        }

        if (!cmdNode.empty()) {
            out << "    " << root << " -> " << cmdNode << ";\n";
        }
    }

    out << "}\n";
    out.close();

    std::cout << "AST exportado a " << filename << std::endl;
}