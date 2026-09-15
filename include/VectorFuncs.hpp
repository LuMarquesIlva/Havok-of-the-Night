#pragma once

#include <cstddef>
#include <type_traits>
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
    size_t size = 0;
    std::string type_name;
public:

    template<typename... Args>
    Internal_Vector(Args... args) : size(sizeof...(args)), type_name(GetType(args...))
    {
        if (this->type_name[0] == 'b') { // Reserves space for boolean values and pushes them into the vector
            this->b_value.reserve(this->size);
            ((this->b_value.push_back(args)), ...);
        } else if (this->type_name[0] == 'i') { // Reserves space for integer values and pushes them into the vector
            this->i_value.reserve(this->size);
            ((this->i_value.push_back(args)), ...);
        } else if (this->type_name[0] == 'f') { // Reserves space for float values and pushes them into the vector
            this->f_value.reserve(this->size);
            ((this->f_value.push_back(args)), ...);
        }
    }

    void Append(bool value)
    {
        if (this->type_name[0] != 'b')
            throw std::invalid_argument("Type mismatch");

        this->b_value.push_back(value);
        this->size++;
    }

    void Append(int value)
    {
        if (this->type_name[0] != 'i')
            throw std::invalid_argument("Type mismatch");

        this->i_value.push_back(value);
        this->size++;
    }

    void Append(float value)
    {
        if (this->type_name[0] != 'f')
            throw std::invalid_argument("Type mismatch");

        this->f_value.push_back(value);
        this->size++;
    }

    void Set(size_t index, bool value)
    {
        if (index < 0 || index >= this->size)
            throw std::out_of_range("Index out of range");

        if (this->type_name[0] != 'b')
            throw std::invalid_argument("Type mismatch | expected bool, got " + this->type_name);

        // Sets the value at index to value, resizing if necessary
        if (this->size == 0) {
            this->b_value.push_back(value);
        } else {
            this->b_value[index] = value;
        }
        return;
    }

    void Set(size_t index, int value)
    {
        if (index < 0 || index >= this->size)
            throw std::out_of_range("Index out of range");

        if (this->type_name[0] != 'i')
            throw std::invalid_argument("Type mismatch | expected int, got " + this->type_name);

        if (this->size == 0) {
            this->i_value.push_back(value);
        } else {
            this->i_value[index] = value;
        }
        return;
    }

    void Set(size_t index, float value)
    {
        if (index < 0 || index >= this->size)
            throw std::out_of_range("Index out of range");

        if (this->type_name[0] != 'f')
            throw std::invalid_argument("Type mismatch | expected float, got " + this->type_name);

        if (this->size == 0) {
            this->f_value.push_back(value);
        } else {
            this->f_value[index] = value;
        }
        return;
    }

    template<typename... T>
    std::string GetType(T... args) const // Returns the type name of the arguments
    {

        std::string_view typeVar = get_type_name(args...);

        if (debug_var == true) {
            SDL_Log("%s", typeVar.data());
        }

        // Get only the argument types by offsetting with <find>
        size_t typeStart = typeVar.find("T = ") + 2;
        size_t typeEnd = typeVar.find("}");
        std::string type_name(typeVar.begin() + typeStart, typeVar.begin() + typeEnd);

        std::vector<std::string> type_name_vec = split(type_name, ",");

        if (debug_var == true) {
            print_vector(type_name_vec);
        }

        if (type_name.empty())
            return "null";;

        if (strcmp(type_name_vec[0].c_str(), "bool") == 0)
            return "bool";
        else if (strcmp(type_name_vec[0].c_str(), "int") == 0)
            return "int";
        else if (strcmp(type_name_vec[0].c_str(), "float") == 0)
            return "float";

        throw std::invalid_argument("Unknown type in <Internal_Vector>");
    }

    bool GetBool(size_t index) const
    {
        if (index < 0 || index >= this->size)
            throw std::out_of_range("Index out of range");
        return this->b_value[index];
    }

    int GetInt(size_t index) const
    {
        if (index < 0 || index >= this->size)
            throw std::out_of_range("Index out of range");
        return this->i_value[index];
    }

    float GetFloat(size_t index) const
    {
        if (index < 0 || index >= this->size)
            throw std::out_of_range("Index out of range");
        return this->f_value[index];
    }

    size_t GetSize() const
    {
        return this->size;
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
