#ifndef COMMAND_H
#define COMMAND_H

#include "../math/vector2.h"

struct Command
{
    uint32_t sequence_number, tick;
    V2 move_dir, aim_target;
    bool is_attacking;
};
#endif