#pragma once

#include <cxxabi.h>
#include <string_view>
#include <string>
#include <vector>
#include <iostream>

#include <SDL3/SDL_log.h>

static bool debug_var = false;

template<typename... T>
static constexpr std::string_view get_type_name(T...)
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

class Utility {
    public:
    #ifdef _USE_VULKAN
        static uint8_t* load_spirv_file(const char* filename, size_t* out_size);
    #endif

    static std::vector<std::string> split(const std::string& str, const std::string& delim);

    static void print_vector(std::vector<std::string> v);
};
