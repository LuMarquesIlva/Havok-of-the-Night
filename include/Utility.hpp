#pragma once

#include <cstdint>
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

uint8_t* load_spirv_file(const char* filename, size_t* out_size) {
    // Open file in binary mode
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        throw std::runtime_error(std::string("Failed to open file: ") + filename);
    }

    // Get file size
    std::streampos size = file.tellg();
    file.seekg(0, std::ios::beg);

    // Allocate memory for uint8_t array
    uint8_t* buffer = new uint8_t[size];

    // Read binary data
    if (file.read(reinterpret_cast<char*>(buffer), size)) {
        *out_size = static_cast<size_t>(size);
        return buffer;
    } else {
        delete[] buffer;
        throw std::runtime_error("Failed to read file contents");
    }
}

std::vector<Uint8*> LoadFile_B(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
        if (!file.is_open()) {
            return {};
        }

        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);

        std::vector<Uint8*> buffer(size);
        if (file.read(reinterpret_cast<char*>(buffer.data()), size)) {
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
