#ifndef VECTOR2_H
#define VECTOR2_H

#include <cmath>

struct V2
{
    float x = 0.0f;
    float y = 0.0f;

    V2 operator+(const V2& v) const
    {
        return {x + v.x, y + v.y};
    }
    V2 operator-(const V2& v) const
    {
        return {x - v.x, y - v.y};
    }
    V2 operator*(const V2& v) const
    {
        return {x * v.x, y * v.y};
    }
    V2 operator*(float scalar) const
    {
        return {x * scalar, y * scalar};
    }

    float length() const { return std::sqrt(x * x + y * y);}
};
#endif