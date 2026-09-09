#pragma once

#include <cstddef>
#include <vector>
#include <iostream>
#include <stdexcept>
#include <string>

#include "Utility.hpp"


class Internal_Vector
{
private:
    std::vector<bool> b_value = {};
    std::vector<int> i_value = {};
    std::vector<float> f_value = {};
    std::string type_name;
    int size = 0;
public:

    template<typename... Args>
    Internal_Vector(Args... args) : size(sizeof...(args)), type_name(GetType(args...))
    {

        SDL_Log("%s", type_name);

        if (type_name[0] == 'b') {
            ((b_value.push_back(args)), ...);
        } else if (type_name[0] == 'i') {
            ((i_value.push_back(args)), ...);
        } else if (type_name[0] == 'f') {
            ((f_value.push_back(args)), ...);
        }
    }

    void Set(int index, int value)
    {
        this->i_value[index] = value;
    }

    void Set(int index, float value)
    {
        this->f_value[index] = value;
    }

    template<typename... Args>
    std::string GetType(Args... args) const
    {
        const char* type_name = get_type_name(args...);
        if (strcmp(type_name, "b") == 0)
            return typeid(b_value).name();
        if (strcmp(type_name, "i") == 0)
            return typeid(i_value).name();
        return typeid(f_value).name();
    }

    bool GetBool(int index) const
    {
        if (index < 0 || index >= size)
            throw std::out_of_range("Index out of range");
        return b_value[index];
    }

    int GetInt(int index) const
    {
        if (index < 0 || index >= size)
            throw std::out_of_range("Index out of range");
        return i_value[index];
    }

    float GetFloat(int index) const
    {
        if (index < 0 || index >= size)
            throw std::out_of_range("Index out of range");
        return f_value[index];
    }

    size_t GetSize() const
    {
        return size;
    }

};

class Vector2
{
public:
    float x = 0.0;
    float y = 0.0;

    // Vector Class Constructor: Uses the two values to create a Vector2 type;
    // the values can be accessed trought Vector2.x and Vector2.y
    Vector2(float value1=0.0, float value2=0.0) : x(value1), y(value2)
    {
        try {
            std::vector<float> Vector2(value1, value2);
        } catch (const std::length_error& e) {
            std::cerr << "Vector size exceeds theoretical max: " << e.what() << " -> Zeroing\n";
            value1 = 0.0f;
            value2 = 0.0f;
        }
    };

    Vector2 operator+=(float rhs) const {
        return Vector2(this->x + rhs, this->y + rhs);
    }

    Vector2 operator+=(const Vector2& rhs) const {
        return Vector2(this->x + rhs.x, this->y + rhs.y);
    }

};
