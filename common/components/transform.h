#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "../math/vector2.h"

struct TransformComponent
{
    V2 position{0.0f, 0.0f};
    float rotation = 0.0f;
};
#endif