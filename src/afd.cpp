#include "afd.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include <iostream>

using json = nlohmann::json;

// carga la definicion del AFD desde un archivo json
void AFD::load_from_file(const std::string& filename) {
    std::ifstream file(filename);
    json j;
    file >> j;

    // cargar estado inicial
    initial_state = j["estado_inicial"].get<std::string>();

    // cargar estados finales
    final_states = j["finales"].get<std::unordered_map<std::string, std::string>>();

    // cargar transiciones entre estados
    transitions = j["transiciones"].get<std::unordered_map<std::string, std::unordered_map<std::string, std::string>>>();

    // cargar el alfabeto clasificado por categorias
    alphabet_map = j["alfabeto"].get<std::unordered_map<std::string, std::string>>();

    // si existen estados de trampa, los agrega
    if (j.contains("estados_trampado")) {
        for (auto& [k, _] : j["estados_trampado"].items()) {
            trap_states.insert(k);
        }
    }

    // si existen estados de error, los agrega
    if (j.contains("estados_error")) {
        for (auto& [k, _] : j["estados_error"].items()) {
            error_states.insert(k);
        }
    }
}

// clasifica un caracter segun el estado actual
std::string AFD::classify(char c, const std::string& state) const {
    // si estamos en un estado que espera digitos hexadecimales
    if (state == "q11" || state == "q12" || state == "q13" || state == "q14" || state == "q15" || state == "q16") {
        std::string hex = "0123456789abcdefABCDEF";
        if (hex.find(c) != std::string::npos) return "HEXADECIMAL";
    }

    // letras con y sin acentos (aunque no usamos acentos aqui)
    std::string letras_ext = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZáéíóúÁÉÍÓÚñÑ";
    if (letras_ext.find(c) != std::string::npos) return "letra";

    // digitos numericos
    if (std::isdigit(c)) return "NUMERO";

    // otros simbolos especiales
    if (c == '_') return "guion_bajo";
    if (c == '"') return "comillas";
    if (c == '#') return "numeral";
    if (c == '/') return "barra";
    if (c == '\n') return "salto";
    if (c == ' ') return "espacio";

    // simbolos ascii comunes
    std::string simbolo = "()!$,%&'*+-.:;<=>?@[\\]^`{|}~";
    if (simbolo.find(c) != std::string::npos) return "SIMBOLO";

    // si no pertenece a ninguna categoria conocida
    return "otro";
}

// ejecuta el AFD sobre la cadena de entrada
std::string AFD::run(const std::string& input, size_t& length_matched) const {
    std::string current_state = initial_state;
    std::string last_accept_state;
    size_t last_accept_pos = 0;

    // RECORRE la cadena caracter por caracter
    for (size_t i = 0; i < input.size(); ++i) {
        std::string category = classify(input[i], current_state);
        
        // std::cout << "Estado actual: " << current_state << std::endl;
        // std::cout << "Caracter: " << input[i] << " -> Categoria: " << category << std::endl;
        
        // verifica si existe transicion valida
        if (transitions.count(current_state) &&
            transitions.at(current_state).count(category)) {
            current_state = transitions.at(current_state).at(category);
            // std::cout << "Transicion: " << current_state << std::endl;
        } else {
            // std::cout << "NO HAY transicion valida. Estado actual: " << current_state << std::endl;
            break;
        }

        // guarda el ultimo estado de aceptacion alcanzado
        if (final_states.count(current_state)) {
            last_accept_state = current_state;
            last_accept_pos = i + 1;
            // std::cout << "Estado de ACEPTACION: " << current_state << std::endl;
        }

        // si cae en un estado trampa o error, se detiene
        if (trap_states.count(current_state) || error_states.count(current_state)) {
            // std::cout << "Estado TRAMPA o de ERROR alcanzado: " << current_state << std::endl;
            break;
        }
    }

    // std::cout << "FIN de la cadena. Estado final: " << current_state << std::endl << std::endl;

    // si se alcanzo un estado de aceptacion, devuelve su token
    if (!last_accept_state.empty()) {
        length_matched = last_accept_pos;
        return final_states.at(last_accept_state);
    }

    // si NO se acepto nada, se considera desconocido
    length_matched = 1;
    return "DESCONOCIDO";
}