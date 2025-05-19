/**
 * @file afd.h
 */
#ifndef AFD_H
#define AFD_H

#include <unordered_map>
#include <unordered_set>
#include <string>
#include <vector>

struct AFD {
    std::string initial_state;
    std::unordered_map<std::string, std::string> final_states;
    std::unordered_set<std::string> trap_states;
    std::unordered_set<std::string> error_states;
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> transitions;
    std::unordered_map<std::string, std::string> alphabet_map;

    void load_from_file(const std::string& filename);
    std::string classify(char c, const std::string& state) const;
    std::string run(const std::string& input, size_t& length_matched) const;

};

#endif // AFD_H