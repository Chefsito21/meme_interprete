#ifndef AST_H
#define AST_H

#include <string>
#include <vector>
#include <memory>

class ASTNode {
public:
    virtual ~ASTNode() = default;
};

class PonerImagen : public ASTNode {
public:
    std::string ruta;
    PonerImagen(const std::string& ruta) : ruta(ruta) {}
};

class CambiarColorTexto : public ASTNode {
public:
    std::string color;
    CambiarColorTexto(const std::string& color) : color(color) {}
};

class CambiarTamanoTexto : public ASTNode {
public:
    int tamano;
    CambiarTamanoTexto(int tamano) : tamano(tamano) {}
};

class Escribe : public ASTNode {
public:
    std::string texto;
    std::string posicion;
    Escribe(const std::string& texto, const std::string& posicion)
        : texto(texto), posicion(posicion) {}
};

class EscribeMultilinea : public ASTNode {
public:
    std::string texto1;
    std::string texto2;
    std::string posicion;
    EscribeMultilinea(const std::string& t1, const std::string& t2, const std::string& pos)
        : texto1(t1), texto2(t2), posicion(pos) {}
};

class GuardaComo : public ASTNode {
public:
    std::string ruta;
    GuardaComo(const std::string& ruta) : ruta(ruta) {}
};

#endif // AST_H
