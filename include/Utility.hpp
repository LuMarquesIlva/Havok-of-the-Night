#pragma once

#include <typeinfo>
#include <cxxabi.h>
#include <iostream>

#include <SDL3/SDL_log.h>

static bool debug_var = false;

template<typename... Args>
const char* get_type_name(Args... args)
{
    const char* type_name = ((typeid(*args).name()), ...);
    int status;
    char* demangled = abi::__cxa_demangle(type_name, 0, 0, &status);
    if (status == 0)
        type_name = demangled;
    if (strcmp(type_name, "bool") == 0)
        return typeid(bool).name();
    else if (strcmp(type_name, "int") == 0)
        return typeid(int).name();
    else if (strcmp(type_name, "float") == 0)
        return typeid(float).name();
    throw std::runtime_error("Unknown type on Utility function");
}
