#ifndef ICONTROLLER_H
#define ICONTROLLER_H

struct Command;
struct WorldContext;

class IController
{
public:
    virtual Command getCommand(const WorldContext& world_context) = 0;

    virtual ~IController() = default;
};
#endif