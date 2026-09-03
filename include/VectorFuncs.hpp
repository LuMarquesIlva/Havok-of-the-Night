#pragma once

#include <vector>
#include <iostream>
#include <stdexcept>

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
