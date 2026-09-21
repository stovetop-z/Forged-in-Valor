#ifndef MOVEMENT_H
#define MOVEMENT_H

#include "../math/vector2.h"

struct MovementComponent
{
    V2 velocity{0.0f, 0.0f};
    float max_speed = 200.0f;
};
#endif