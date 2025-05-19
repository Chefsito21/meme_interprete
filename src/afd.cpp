#include "afd.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include <iostream>

using json = nlohmann::json;



void AFD::load_from_file(const std::string& filename) {
    std::ifstream file(filename);
    json j;
    file >> j;

    initial_state = j["estado_inicial"].get<std::string>();
    final_states = j["finales"].get<std::unordered_map<std::string, std::string>>();
    transitions = j["transiciones"].get<std::unordered_map<std::string, std::unordered_map<std::string, std::string>>>();
    alphabet_map = j["alfabeto"].get<std::unordered_map<std::string, std::string>>();

    if (j.contains("estados_trampado")) {
        for (auto& [k, _] : j["estados_trampado"].items()) {
            trap_states.insert(k);
        }
    }

    if (j.contains("estados_error")) {
        for (auto& [k, _] : j["estados_error"].items()) {
            error_states.insert(k);
        }
    }
}

std::string AFD::classify(char c, const std::string& state) const {
    if (state == "q11" || state == "q12" || state == "q13" || state == "q14" || state == "q15" || state == "q16") {
        std::string hex = "0123456789abcdefABCDEF";
        if (hex.find(c) != std::string::npos) return "HEXADECIMAL";
    }

    std::string letras_ext = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZáéíóúÁÉÍÓÚñÑ";
    if (letras_ext.find(c) != std::string::npos) return "letra";
    if (std::isdigit(c)) return "NUMERO";
    if (c == '_') return "guion_bajo";
    if (c == '"') return "comillas";
    if (c == '#') return "numeral";
    if (c == '/') return "barra";
    if (c == '\n') return "salto";
    if (c == ' ') return "espacio";
    std::string simbolo = "()!$,%&'*+-.:;<=>?@[\\]^`{|}~";
    if (simbolo.find(c) != std::string::npos) return "SIMBOLO";
    return "otro";
}


std::string AFD::run(const std::string& input, size_t& length_matched) const {
    std::string current_state = initial_state;
    std::string last_accept_state;
    size_t last_accept_pos = 0;

    for (size_t i = 0; i < input.size(); ++i) {
        std::string category = classify(input[i], current_state);
        
        std::cout << "Estado actual: " << current_state << std::endl;
        std::cout << "Caracter: " << input[i] << " -> Categoria: " << category << std::endl;
        

        if (transitions.count(current_state) &&
            transitions.at(current_state).count(category)) {
            current_state = transitions.at(current_state).at(category);
            std::cout << "Transición: " << current_state << std::endl;
        } else {
            std::cout << "No hay transición válida. Estado actual: " << current_state << std::endl;
            break;
        }

        if (final_states.count(current_state)) {
            last_accept_state = current_state;
            last_accept_pos = i + 1;
            std::cout << "Estado de aceptación: " << current_state << std::endl;
        }

        if (trap_states.count(current_state) || error_states.count(current_state)) {
            std::cout << "Estado trampa o de error alcanzado: " << current_state << std::endl;
            break;
        }
    }
    std::cout << "Fin de la cadena. Estado final: " << current_state << std::endl << std::endl;

    if (!last_accept_state.empty()) {
        length_matched = last_accept_pos;
        return final_states.at(last_accept_state);
    }

    length_matched = 1;
    // std::cout << "No se encontró un estado final. Estado actual: " << current_state << std::endl;
    return "DESCONOCIDO";
}
