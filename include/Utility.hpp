#pragma once

#include <cxxabi.h>
#include <string_view>
#include <string>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <fstream>


#include <SDL3/SDL_log.h>


static bool debug_var = false;

std::vector<char> LoadFile_B(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
        if (!file.is_open()) {
            return {};
        }

        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);

        std::vector<char> buffer(size);
        if (file.read(buffer.data(), size)) {
            return buffer;
        }
        return {};
}

template<typename... T>
constexpr std::string_view get_type_name(T...)
{
    #if defined(__GNUC__) || defined(__clang__)
        std::string_view func = __PRETTY_FUNCTION__;
        // Format: "constexpr std::string_view get_type_name() [with T = ...]"
        if (debug_var)
            SDL_Log("func_data: %s", func.data());
        size_t start = func.find("T = ") + 4; // Get the T arguments
        return func.substr(start, func.size() - start - 1);
    #elif defined(_MSC_VER)
        std::string_view func = __FUNCSIG__;
        // Format: "class std::basic_string_view<char,struct std::char_traits<char> > __cdecl get_type_name<int>(void)"
        size_t start = func.find("get_type_name<") + 13;
        return func.substr(start, func.find('>') - start);
    #endif
}

std::vector<std::string> split(const std::string& str, const std::string& delim) {
    std::vector<std::string> tokens;
    size_t pos = 0;
    std::string token;
    std::string str_copy = str;
    while ((pos = str_copy.find(delim)) != std::string::npos) {
        token = str_copy.substr(0, pos);
        tokens.push_back(token);
        str_copy.erase(0, pos + delim.length()); // Nota: str_copy é passado por valor
    }

    if (tokens.empty()) {
        throw std::runtime_error("tokens is empty after split");
    }

    tokens.push_back(str_copy);
    return tokens;
}

void print_vector(std::vector<std::string> v) {
    for (const auto& elem : v) {
        std::cout << elem << " |";
    }
    std::cout << "\n";
}
