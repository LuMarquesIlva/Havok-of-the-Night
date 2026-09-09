#pragma once

#include <typeinfo>
#include <cxxabi.h>
#include <iostream>

#include <SDL3/SDL_log.h>

static bool debug_var = false;

template<typename... Args>
const char* get_type_name(Args... args)
{
    const char* type_name = ((typeid(args).name()), ...);
    int status;
    char* demangled = abi::__cxa_demangle(type_name, 0, 0, &status);
    if (status == 0)
        type_name = demangled;
    if (strcmp(type_name, "b") == 0)
        return typeid(bool).name();
    if (strcmp(type_name, "i") == 0)
        return typeid(int).name();
    if (strcmp(type_name, "f") == 0)
        return typeid(float).name();
    return typeid(double).name();
}
