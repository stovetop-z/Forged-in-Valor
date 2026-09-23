#ifndef COLLIDER_H
#define COLLIDER_H

#include "../math/vector2.h"
#include <cstdint>

struct ColliderComponent
{
    V2 size{16.0f, 16.0f};
    V2 offset{0.0f, 0.0f};
    bool isTrigger = false;
};

struct AABB
{
    V2 min, max;
};
#endif