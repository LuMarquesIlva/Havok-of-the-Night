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
    size_t size = 0;
    std::string type_name;
public:

    template<typename... Args>
    Internal_Vector(Args... args) : size(sizeof...(args)), type_name(GetType(args...))
    {
        if (sizeof...(args) == 0)
            return;

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

    void Set(size_t index, bool value)
    {
        if (index < 0 || index >= this->size)
            throw std::out_of_range("Index out of range");

        if (this->type_name[0] != 'b')
            throw std::invalid_argument("Type mismatch");

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
            throw std::invalid_argument("Type mismatch");

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
            throw std::invalid_argument("Type mismatch");

        if (this->size == 0) {
            this->f_value.push_back(value);
        } else {
            this->f_value[index] = value;
        }
        return;
    }

    template<typename... Args>
    std::string GetType(Args... args) const // Returns the type name of the arguments
    {
        const char* type_name = get_type_name(&args...);
        if (strcmp(type_name, "b") == 0)
            return "bool";
        else if (strcmp(type_name, "i") == 0)
            return "int";
        else if (strcmp(type_name, "f") == 0)
            return "float";

        throw std::invalid_argument("Unknown type");
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
